//main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IP_ADDRESS  "127.0.0.1"
#define IP_PORT     "10000"

int main(int argc, char* argv[])
{
    int                 sockfd;
    struct sockaddr_in  sockaddr;
    char                buffer[1024];
    int                 rv;

    printf("TCP client started ...\n");

    //create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("ERROR: Socket creation failed ...\n\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created ...\n");

    //set destination IP address & IP port (server)
    memset(&sockaddr, 0x00, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    sockaddr.sin_port = htons(atoi(IP_PORT));

    //try to connect client to server
    if(connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) != 0)
    {
        printf("ERROR: Connect failed ...\n\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    else
        printf("Connection established ...\n");

    //send data to server
    strcpy(buffer, "REQUEST from client to server");
    rv = write(sockfd, buffer, strlen(buffer) + 1);
    printf("%i bytes of data transmitted: \"%s\" ...\n", rv, buffer);

    //receive data from server
    rv = read(sockfd, buffer, sizeof(buffer));
    printf("%i bytes of data received:    \"%s\" ...\n", rv, buffer);

    //disconnect and close socket
    printf("Terminate connection ...\n");
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    exit(EXIT_SUCCESS);
}
