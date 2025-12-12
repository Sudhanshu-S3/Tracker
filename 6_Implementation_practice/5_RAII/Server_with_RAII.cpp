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

class RedisServer
{
private:
    Socket server_socket;
    Socket epoll_fd;
    std::unordered_map<std::string, std::string> store;
    std::unordered_map<int, std::unique_ptr<Socket>> clients;

    // Helper: Set Non-Blocking
    void set_nonblocking(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    // Parse RESP
    std::vector<std::string> parse_resp(const std::string& raw)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(raw);
        std::string line;

        while (std::getline(ss, line))
        {
            if (!line.empty() && line.back() == '\r') 
                line.pop_back();
            
            if (line.empty() || line[0] == '*' || line[0] == '$') 
                continue;
            
            tokens.push_back(line);
        }

        return tokens;
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
        
        // Store in RAII wrapper - now properly managed!
        clients[raw_client_fd] = std::make_unique<Socket>(raw_client_fd);
        
        std::cout << "New client connected: " << raw_client_fd << std::endl;
    }

    void handle_client_data(int fd)
    {
        char buffer[4096] = {0};
        int bytes = read(fd, buffer, 4096);
        
        if (bytes <= 0)
        {
            std::cout << "Client disconnected: " << fd << std::endl;
            
            // Remove from epoll
            epoll_ctl(epoll_fd.get(), EPOLL_CTL_DEL, fd, nullptr);
            
            // Remove from clients map - Socket destructor will close fd
            clients.erase(fd);
            return;
        }

        std::string raw(buffer, bytes);
        auto cmd = parse_resp(raw);
        
        if (cmd.empty())
            return;
        
        std::string command = cmd[0];
        
        if (command == "PING")
        {
            send(fd, "+PONG\r\n", 7, 0);
        }
        else if (command == "SET" && cmd.size() >= 3)
        {
            store[cmd[1]] = cmd[2];
            send(fd, "+OK\r\n", 5, 0);
        }
        else if (command == "GET" && cmd.size() >= 2)
        {
            if (store.count(cmd[1]))
            {
                std::string val = store[cmd[1]];
                std::string resp = "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";
                send(fd, resp.c_str(), resp.length(), 0);
            }
            else
            {
                send(fd, "$-1\r\n", 5, 0);
            }
        }
        else
        {
            send(fd, "-ERR unknown command\r\n", 22, 0);
        }
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