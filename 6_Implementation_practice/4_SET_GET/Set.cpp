#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <unordered_map>


std::unordered_map< std::string , std:: string> g_store;

std::vector<std::string> parse_resp( const std::string& raw)
{
    std::vector<std::string> tokens;
    std::stringstream ss(raw);
    std::string line;

    while (std::getline(ss, line))
    {
        // Remove \r if present.
        if(!line.empty() && line.back() == '\r') line.pop_back();

        /*
        Lines starting with '*' are array size.
        Lines starting with '$' are string lengths.
        */

        if(line.empty() || line[0] == '*' || line[0] == '$') continue;

        tokens.emplace_back(line);
    }
    return tokens;
}

int set_nonblocking ( int fd )
{
    int flags = fcntl ( fd , F_GETFL , 0 );
    return fcntl (fd , F_SETFL , flags | O_NONBLOCK);
}

int main()
{
    // Server Setup

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    std::cout << "Socket creation (fd :" << server_fd << ")\n";

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(6379);

    int opt = 1;
    setsockopt ( server_fd , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt));
    bind ( server_fd , (struct sockaddr*)& address , sizeof(address));
    listen ( server_fd , SOMAXCONN );

    // Create Epoll instance.

    set_nonblocking(server_fd); 

    int epoll_fd = epoll_create1(0);

    // Add the Server Socket to the WatchList.

    struct epoll_event ev;
    ev.events = EPOLLIN ; 
    ev.data.fd = server_fd ; 
    epoll_ctl ( epoll_fd , EPOLL_CTL_ADD , server_fd, &ev);

    struct epoll_event events[10];
    std::cout << "Epoll Server Listening on 6379... " << std::endl;

    // Accept

    while(true)
    {
        // Sleep until something happens ( -1 == wait forever )
        int nfds = epoll_wait (epoll_fd , events , 10 , -1);

        for ( int i = 0 ; i < nfds ; i++)
        {
            if( events[i].data.fd == server_fd)
            {
                struct sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                int client_fd = accept ( server_fd, (struct sockaddr*)&client_addr , &len);

                // Make the new client non-blocking 
                set_nonblocking ( client_fd );

                //Add the new client to the watchlist
                ev.events  = EPOLLIN | EPOLLET ;  // Edge Triggered
                ev.data.fd = client_fd ;
                epoll_ctl ( epoll_fd, EPOLL_CTL_ADD , client_fd, &ev);

                std::cout << "New Client Connected : " << client_fd <<std::endl;
            }
            else
            {
                // Data from an Existing Client

                // RESP Parsing

                char buffer[4096] = {0};
                int bytes = read( events[i].data.fd , buffer, 4096);

                if(bytes <=0 )
                {
                    close ( events[i].data.fd );
                    std::cout << "Client Disconnected" << std::endl;
                }
                else
                {
                    std::string raw_request(buffer, bytes);
                    std::vector<std::string> command = parse_resp(raw_request);

                    // Use a Loop because null bytes might hide parts of the string

                    if (command.empty()) continue;

                    std::string cmd = command[0];

                    if(cmd == "PING")
                    {
                        const char* resp = "+PONG\r\n";
                        send(events[i].data.fd, resp, 7 , 0);
                    }
                    // ... PING logic remains the same ...
                    
                    // FIXED SET: Write to the Map
                    else if (cmd == "SET" && command.size() >= 3)
                    {
                        g_store[command[1]] = command[2]; // ACTUALLY STORE IT!
                        const char* resp = "+OK\r\n";
                        send(events[i].data.fd, resp, 5, 0);
                    }
                    
                    // MISSING GET: Read from the Map
                    else if (cmd == "GET" && command.size() >= 2)
                    {
                        if (g_store.count(command[1]))
                        {
                            std::string val = g_store[command[1]];
                            std::string resp = "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";
                            send(events[i].data.fd, resp.c_str(), resp.length(), 0);
                        }
                        else
                        {
                            const char* resp = "$-1\r\n"; // Key not found (NULL)
                            send(events[i].data.fd, resp, 5, 0);
                        }
                    }
                    

                    else
                    {
                        const char* resp = "-ERR unknown command\r\n";
                        send(events[i].data.fd , resp, 22 , 0);
                    }
                }
                
            }
        }
    }
    close (server_fd);
    return 0;
}
