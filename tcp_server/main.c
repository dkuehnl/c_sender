//main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IP_PORT     "10000"

int main(int argc, char* argv[])
{
    int                 sockfd, connfd;
    struct sockaddr_in  sockaddr, connaddr;
    unsigned int        connaddr_len;
    char                buffer[1024];
    int                 rv;

    printf("TCP server started ...\n");

    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("ERROR: Socket creation failed ...\n\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created ...\n");

    //set IP address & IP port
    memset(&sockaddr, 0x00, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(atoi(IP_PORT));

    //bind socket
    if((bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) != 0)
    {
        printf("ERROR: Socket bind failed ...\n\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully binded ...\n");

    //listen to socket
    if((listen(sockfd, 5)) != 0)
    {
        printf("ERROR: Listen failed ...\n\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    else
        printf("Server listening ...\n\n");

    //server loop
    while(1)
    {
        connaddr_len = sizeof(connaddr);
        connfd = accept(sockfd, (struct sockaddr*)&connaddr, &connaddr_len);
        if(connfd < 0)
        {
            printf("ERROR: Server accept failed ...\n\n");
            break;
        }
        else
            printf("New connection accepted ...\n");

        //connection loop
        while(1)
        {
            //try to read from socket
            rv = read(connfd, buffer, sizeof(buffer));

            //close connection if error detected
            if(rv < 1)
            {
                printf("Connection closed ...\n\n");
                close(connfd);
                break;
            }
            else
                printf("%i bytes of data received:    \"%s\" ...\n", rv, buffer);

            strcpy(buffer, "RESPONSE from server to client");
            rv = write(connfd, buffer, strlen(buffer) + 1);
            printf("%i bytes of data transmitted: \"%s\" ...\n", rv, buffer);
        }
    }
}
