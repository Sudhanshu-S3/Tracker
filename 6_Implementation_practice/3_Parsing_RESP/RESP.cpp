#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>




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

    int epoll_fd = epoll_create1 ( 0 );

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

                char buffer[1024] = {0};
                int bytes = read( events[i].data.fd , buffer, 1024);

                if(bytes <=0 )
                {
                    close ( events[i].data.fd );
                    std::cout << "Client Disconnected" << std::endl;
                }
                else
                {
                    // Use a Loop because null bytes might hide parts of the string

                    std::cout << "Received Raw Bytes: ";
                    for ( int j =0 ; j < bytes ; j++)
                    {
                        if ( buffer[j] == '\r') std::cout <<"\\r";
                        else if(buffer[j] == '\n' ) std::cout << "\\n";
                        else std::cout << buffer[j];
                    }

                    std::cout << std::endl;

                    /*
                    Check if the buffer contains "PING".
                    Only the temparary PING parse.
                    */

                    std::string request(buffer , bytes);

                    if( request.find("PING") != std::string::npos)
                    {
                        const char* response = "+PONG\r\n";
                        send ( events[i].data.fd , response , 7 , 0 );
                    }
                    else{
                        const char* err = "-ERR unknown command\r\n";
                        send( events[i].data.fd, err , 22 , 0);
                    }
                }

                
            }
        }
    }
    close (server_fd);
    return 0;
}
