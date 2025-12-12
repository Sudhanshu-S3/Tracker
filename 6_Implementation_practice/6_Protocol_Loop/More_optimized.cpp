#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cstring>
#include <cerrno>

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

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

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

// Ultra-fast RESP parser - zero allocations
struct RESPParser
{
    const char* data;
    size_t len;
    size_t pos;
    
    RESPParser(const char* d, size_t l) : data(d), len(l), pos(0) {}
    
    // Fast integer parsing - inline and branchless where possible
    inline int parse_int(size_t& p) 
    {
        int result = 0;
        bool negative = (p < len && data[p] == '-');
        if (negative) p++;
        
        while (p < len && data[p] >= '0' && data[p] <= '9') {
            result = result * 10 + (data[p] - '0');
            p++;
        }
        
        return negative ? -result : result;
    }
    
    // Parse command - returns pointers and lengths instead of allocating strings
    struct Token 
    {
        const char* ptr;
        int len;
    };
    
    size_t try_parse_command(Token* tokens, int& token_count, int max_tokens) 
    {
        token_count = 0;
        size_t start = pos;
        
        if (pos >= len || data[pos] != '*') return 0;
        pos++;
        
        int array_size = parse_int(pos);
        if (pos + 1 >= len || data[pos] != '\r' || data[pos + 1] != '\n') 
        {
            pos = start;
            return 0;
        }
        pos += 2;
        
        if (array_size > max_tokens) 
        {
            pos = start;
            return 0;
        }
        
        for (int i = 0; i < array_size; i++) 
        {
            if (pos >= len || data[pos] != '$') 
            {
                pos = start;
                return 0;
            }
            pos++;
            
            int str_len = parse_int(pos);
            if (pos + 1 >= len || data[pos] != '\r' || data[pos + 1] != '\n') 
            {
                pos = start;
                return 0;
            }
            pos += 2;
            
            if (pos + str_len + 2 > len) 
            {
                pos = start;
                return 0;
            }
            
            tokens[token_count].ptr = data + pos;
            tokens[token_count].len = str_len;
            token_count++;
            
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
    
    struct ClientBuffer 
    {
        char data[16384]; // 16KB buffer
        size_t len = 0;
        char response_buf[16384]; // Response batching buffer
        size_t response_len = 0;
    };
    std::unordered_map<int, ClientBuffer> client_buffers;

    void set_nonblocking(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
    
    // Fast string comparison
    inline bool str_eq(const char* a, int len_a, const char* b, int len_b) 
    {
        return len_a == len_b && memcmp(a, b, len_a) == 0;
    }
    
    // Fast string comparison with literal
    inline bool str_eq(const char* a, int len_a, const char* lit) 
    {
        int len_b = strlen(lit);
        return len_a == len_b && memcmp(a, lit, len_a) == 0;
    }

public:
    RedisServer(int port)
    {
        int raw_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (raw_fd == -1)
        {
            throw std::runtime_error("Socket creation failed");
        }
        server_socket = Socket(raw_fd);
        
        struct sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        int opt = 1;
        setsockopt(server_socket.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        setsockopt(server_socket.get(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
        setsockopt(server_socket.get(), IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
        
        if (bind(server_socket.get(), (struct sockaddr*)&address, sizeof(address)) == -1)
        {
            throw std::runtime_error("Bind failed");
        }
        
        listen(server_socket.get(), SOMAXCONN);
        set_nonblocking(server_socket.get());

        int raw_epoll = epoll_create1(0);
        if (raw_epoll == -1)
        {
            throw std::runtime_error("Epoll creation failed");
        }
        epoll_fd = Socket(raw_epoll);
        
        struct epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.fd = server_socket.get();
        epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, server_socket.get(), &ev);
        
        std::cout << "High-Performance Server listening on port " << port << std::endl;
    }

    void run()
    {
        struct epoll_event events[128]; // Increased from 10
        
        while (true)
        {
            int nfds = epoll_wait(epoll_fd.get(), events, 128, -1);
            
            for (int i = 0; i < nfds; ++i)
            {
                if (events[i].data.fd == server_socket.get())
                {
                    // Accept multiple connections in one go
                    while (true) 
                    {
                        int client = accept(server_socket.get(), nullptr, nullptr);
                        if (client == -1) break;
                        
                        set_nonblocking(client);
                        
                        int opt = 1;
                        setsockopt(client, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
                        
                        struct epoll_event ev{};
                        ev.events = EPOLLIN | EPOLLET;
                        ev.data.fd = client;
                        epoll_ctl(epoll_fd.get(), EPOLL_CTL_ADD, client, &ev);
                        
                        clients[client] = std::make_unique<Socket>(client);
                        client_buffers[client] = ClientBuffer{};
                    }
                }
                else
                {
                    handle_client_data(events[i].data.fd);
                }
            }
        }
    }

private:
    void handle_client_data(int fd)
    {
        ClientBuffer& buf = client_buffers[fd];
        
        // Edge-triggered: read all available data
        while (true)
        {
            int bytes = read(fd, buf.data + buf.len, sizeof(buf.data) - buf.len);
            
            if (bytes <= 0) {
                if (bytes == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) 
                {
                    // Connection closed or error
                    epoll_ctl(epoll_fd.get(), EPOLL_CTL_DEL, fd, nullptr);
                    clients.erase(fd);
                    client_buffers.erase(fd);
                }
                break;
            }
            
            buf.len += bytes;
        }
        
        // Process all complete commands
        RESPParser parser(buf.data, buf.len);
        RESPParser::Token tokens[16];
        int token_count;
        
        buf.response_len = 0; // Reset response buffer
        
        while (true)
        {
            size_t consumed = parser.try_parse_command(tokens, token_count, 16);
            
            if (consumed == 0) 
            {
                if (parser.pos > 0) 
                {
                    memmove(buf.data, buf.data + parser.pos, buf.len - parser.pos);
                    buf.len -= parser.pos;
                }
                break;
            }
            
            if (token_count > 0)
            {
                const char* cmd = tokens[0].ptr;
                int cmd_len = tokens[0].len;
                
                // Fast command dispatch
                if (str_eq(cmd, cmd_len, "PING"))
                {
                    append_response(buf, "+PONG\r\n", 7);
                }
                else if (str_eq(cmd, cmd_len, "SET") && token_count >= 3)
                {
                    std::string key(tokens[1].ptr, tokens[1].len);
                    std::string val(tokens[2].ptr, tokens[2].len);
                    store[std::move(key)] = std::move(val);
                    append_response(buf, "+OK\r\n", 5);
                }
                else if (str_eq(cmd, cmd_len, "GET") && token_count >= 2)
                {
                    std::string key(tokens[1].ptr, tokens[1].len);
                    auto it = store.find(key);
                    
                    if (it != store.end())
                    {
                        const std::string& val = it->second;
                        
                        // Build response directly in buffer
                        buf.response_buf[buf.response_len++] = '$';
                        buf.response_len += fast_int_to_str(val.length(), 
                                                            buf.response_buf + buf.response_len);
                        buf.response_buf[buf.response_len++] = '\r';
                        buf.response_buf[buf.response_len++] = '\n';
                        memcpy(buf.response_buf + buf.response_len, val.data(), val.length());
                        buf.response_len += val.length();
                        buf.response_buf[buf.response_len++] = '\r';
                        buf.response_buf[buf.response_len++] = '\n';
                    }
                    else
                    {
                        append_response(buf, "$-1\r\n", 5);
                    }
                }
                else
                {
                    append_response(buf, "-ERR unknown command\r\n", 22);
                }
            }
        }
        
        // Flush all responses at once
        if (buf.response_len > 0) 
        {
            send(fd, buf.response_buf, buf.response_len, 0);
        }
    }
    
    inline void append_response(ClientBuffer& buf, const char* data, size_t len) 
    {
        memcpy(buf.response_buf + buf.response_len, data, len);
        buf.response_len += len;
    }
    
    // Fast integer to string conversion
    inline int fast_int_to_str(int num, char* buf) 
    {
        if (num == 0) 
        {
            buf[0] = '0';
            return 1;
        }
        
        char temp[12];
        int len = 0;
        int n = num;
        
        while (n > 0) 
        {
            temp[len++] = '0' + (n % 10);
            n /= 10;
        }
        
        for (int i = 0; i < len; i++)
        {
            buf[i] = temp[len - 1 - i];
        }
        
        return len;
    }
};

int main()
{
    try
    {
        RedisServer server(6379);
        server.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}