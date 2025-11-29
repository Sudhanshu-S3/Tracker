#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 500

int main (int argc , char *argv[])
{
    int                     sfd, s;
    char                    buf[BUF_SIZE];
    ssize_t                 nread;
    socklen_t               peer_addrlen;
    struct addrinfo         hints;
    struct addrinfo         *result, *rp;
    struct sockaddr_storage peer_addr;


    if(argc != 2 )
    {
        //fprintf( stderr , "Usage: %s port\n", argv[0]);
        //exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_DGRAM;
    hints.ai_flags      = AI_PASSIVE;
    hints.ai_protocol   = 0;
    hints.ai_canonname  = NULL;
    hints.ai_addr       = NULL;
    hints.ai_next       = NULL;




}