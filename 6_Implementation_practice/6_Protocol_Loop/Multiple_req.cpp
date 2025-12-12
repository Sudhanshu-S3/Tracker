#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

// RAII wrapper for file descriptors
struct Socket
{
    int fd = -1;

    Socket() = default;

    explicit Socket(int raw_fd) : fd(raw_fd) {}

    ~Socket()
    {
        if (fd != -1)
        {
            close(fd);
        }
    }

    // Delete Copy Constructor (Unique Ownership)
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // Allow Move
    Socket(Socket&& other) noexcept : fd(other.fd)
    {
        other.fd = -1;
    }

    Socket& operator=(Socket&& other) noexcept
    {
        if (this != &other)
        {
            if (fd != -1)
            {
                close(fd);
            }
            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }

    int get() const { return fd; }
};

// High-performance RESP parser that works on raw buffer
struct RESPParser
{
    const char* data;
    size_t len;
    size_t pos;
    
    RESPParser(const char* d, size_t l) : data(d), len(l), pos(0) {}
    
    // Fast integer parsing
    inline int parse_int(size_t& p) {
        int result = 0;
        bool negative = false;
        
        if (p < len && data[p] == '-') {
            negative = true;
            p++;
        }
        
        while (p < len && data[p] >= '0' && data[p] <= '9') {
            result = result * 10 + (data[p] - '0');
            p++;
        }
        
        return negative ? -result : result;
    }
    
    // Returns number of bytes consumed, or 0 if incomplete
    size_t try_parse_command(std::vector<std::string_view>& tokens) 
    {
        tokens.clear();
        size_t start = pos;
        
        if (pos >= len) return 0;
        if (data[pos] != '*') return 0;
        pos++;
        
        int array_size = parse_int(pos);
        if (pos + 1 >= len || data[pos] != '\r' || data[pos + 1] != '\n') { pos = start; return 0; }
        pos += 2;
        
        for (int i = 0; i < array_size; i++) {
            if (pos >= len || data[pos] != '$') { pos = start; return 0; }
            pos++;
            
            int str_len = parse_int(pos);
            if (pos + 1 >= len || data[pos] != '\r' || data[pos + 1] != '\n') { pos = start; return 0; }
            pos += 2;
            
            if (pos + str_len + 2 > len) { pos = start; return 0; }
            
            // ZERO COPY MAGIC HERE:
            // Instead of making a string, we make a view.
            tokens.emplace_back(data + pos, str_len);
            
            pos += str_len + 2; 
        }
        return pos - start; 
    }
};

class RedisServer
{
private:
    Socket server_socket;
    Socket epoll_fd;
    std::unordered_map<std::string, std::string> store;
    std::unordered_map<int, std::unique_ptr<Socket>> clients;
    
    // Per-client buffers for partial reads
    struct ClientBuffer {
        char data[8192];
        size_t len = 0;
    };
    std::unordered_map<int, ClientBuffer> client_buffers;

    // Helper: Set Non-Blocking
    void set_nonblocking(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

public:
    RedisServer(int port)
    {
        // 1. Create Socket
        int raw_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (raw_fd == -1)
        {
            throw std::runtime_error("Socket creation failed");
        }
        server_socket = Socket(raw_fd);
        
        // 2. Bind & Listen
        struct sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        int opt = 1;
        setsockopt(server_socket.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        if (bind(server_socket.get(), (struct sockaddr*)&address, sizeof(address)) == -1)
        {
            throw std::runtime_error("Bind failed");
        }
        
        listen(server_socket.get(), SOMAXCONN);
        set_nonblocking(server_socket.get());

        // 3. Create Epoll
        int raw_epoll = epoll_create1(0);
        if (raw_epoll == -1)
        {
            throw std::runtime_error("Epoll creation failed");
        }
        epoll_fd = Socket(raw_epoll);
        
        // 4. Add Server to Epoll
        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.fd = server_socket.get();
        epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, server_socket.get(), &ev);
        
        std::cout << "Server (RAII) listening on port " << port << std::endl;
    }

    void run()
    {
        struct epoll_event events[10];
        
        while (true)
        {
            int nfds = epoll_wait(epoll_fd.get(), events, 10, -1);
            
            for (int i = 0; i < nfds; ++i)
            {
                if (events[i].data.fd == server_socket.get())
                {
                    handle_new_connection();
                }
                else
                {
                    handle_client_data(events[i].data.fd);
                }
            }
        }
    }

private:
    void handle_new_connection()
    {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int raw_client_fd = accept(server_socket.get(), (struct sockaddr*)&client_addr, &len);
        
        if (raw_client_fd == -1)
            return;

        set_nonblocking(raw_client_fd);
        
        // Add to epoll
        struct epoll_event ev{};
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = raw_client_fd;
        epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, raw_client_fd, &ev);
        
        // Store in RAII wrapper
        clients[raw_client_fd] = std::make_unique<Socket>(raw_client_fd);
        
        // Initialize buffer
        client_buffers[raw_client_fd] = ClientBuffer{};
        
        std::cout << "New client connected: " << raw_client_fd << std::endl;
    }

    void handle_client_data(int fd)
    {
        ClientBuffer& buf = client_buffers[fd];
        
        // Read new data into buffer
        int bytes = read(fd, buf.data + buf.len, sizeof(buf.data) - buf.len);
        
        if (bytes <= 0)
        {
            std::cout << "Client disconnected: " << fd << std::endl;
            epoll_ctl(epoll_fd.get(), EPOLL_CTL_DEL, fd, nullptr);
            clients.erase(fd);
            client_buffers.erase(fd);
            return;
        }
        
        buf.len += bytes;
        
        // Process all complete commands in buffer
        RESPParser parser(buf.data, buf.len);
        std::vector<std::string_view> tokens;

        std::string response_buffer; 
        response_buffer.reserve(4096);
        
        while (true)
        {
            size_t consumed = parser.try_parse_command(tokens);
            
            if (consumed == 0) {
                if (parser.pos > 0) {
                    memmove(buf.data, buf.data + parser.pos, buf.len - parser.pos);
                    buf.len -= parser.pos;
                }
                break;
            }
            
            if (!tokens.empty())
            {
                const std::string_view cmd = tokens[0];
                
                if (cmd == "PING") {
                    response_buffer.append("+PONG\r\n");
                }
                else if (cmd == "SET" && tokens.size() >= 3) {
                    // Need to convert view to string for map key (Map requires ownership)
                    store[std::string(tokens[1])] = std::string(tokens[2]);
                    response_buffer.append("+OK\r\n");
                }
                else if (cmd == "GET" && tokens.size() >= 2) {
                    // Look up using string_view (C++20 feature, or manual cast)
                    // For C++17, we construct a temporary string key
                    std::string key(tokens[1]); 
                    auto it = store.find(key);
                    if (it != store.end()) {
                        const std::string& val = it->second;
                        response_buffer.append("$").append(std::to_string(val.length())).append("\r\n").append(val).append("\r\n");
                    } else {
                        response_buffer.append("$-1\r\n");
                    }
                }
                else {
                    response_buffer.append("-ERR unknown command\r\n");
                }
            }
        }
                
        // NEW: Send everything in ONE System Call
        if (!response_buffer.empty()) 
        {
            send(fd, response_buffer.data(), response_buffer.size(), 0);
        }
    }
};

int main()
{
    RedisServer server(6379);
    server.run();

    return 0;
}