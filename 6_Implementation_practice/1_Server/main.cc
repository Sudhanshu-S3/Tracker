#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    // Creating socket => man 2 socket for more detail all this in linux bash terminal

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    std::cout << "Socket creation (fd: " << server_fd << ")\n";

    int opt = 1;
    setsockopt( server_fd, SOL_SOCKET, SO_REUSEADDR, &opt ,sizeof(opt));


    // Bind the Address to the socket => man 2 bind

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Socket bound to port 8080\n";

    // Listen => man 2 listen  

    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Listen for connections ,,,, \n";

    // Accept => waiting for and to accept the incoming connection

    while(true)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        std::cout << "\nWaiting for client ...\n";
        int client_fd = accept( server_fd, (struct sockaddr*)&client_addr, &client_len);

        if( client_fd < 0 )
        {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::cout << " Client connected (fd: " << client_fd <<")\n" ;

        // Communicate => Read and write data

        char buffer[1024] = {0};

        ssize_t bytes_read = read ( client_fd, buffer, sizeof(buffer) - 1);

        if ( bytes_read > 0)
        {
            std::cout << "Received: " << buffer << "\n";

            // Echo back to client

            const char* response = "Hello this is sudhanshu's server\n" ;
            write (client_fd , response , strlen(response));
        }

        // Close client connection

        close( client_fd );
        std::cout << " Client disconnected\n";
        
    }

    // CleanUp

    close ( server_fd );
    return 0;
}