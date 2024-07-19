#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/net.h"


#pragma comment(lib, "Ws2_32.lib")

#define BACKLOG 10


/**
 * Get's internet address (IPv4)
 */
void* get_int_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * Get listening socket
 */
SOCKET get_listener_socket(char* port)
{
    SOCKET sockfd = INVALID_SOCKET;;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int yes = 1;


    // Initialize Winsock
    WSADATA wsaData;
    rv = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rv != 0)
    {
        fprintf(stderr, "WSAStartup failed: %d\n", rv);
        return INVALID_SOCKET;
    }

    // Look at local network interfaces to find IPv4 or IPv6 and TCP

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Go through list of potential interfaces and try to set a socket on each
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET)
            continue;

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)) == SOCKET_ERROR)
        {
            perror("setsckopt");
            closesocket(sockfd);
            freeaddrinfo(servinfo);
            WSACleanup();

            return INVALID_SOCKET;
        }

        // Try to bind socket to IP
        if (bind(sockfd, p->ai_addr, (int)p->ai_addrlen) == SOCKET_ERROR)
        {
            closesocket(sockfd);
            continue;
        }


        // Socket found. No need to continue looping
        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {  
        // No good socket found
        fprintf(stderr, "webserver: failed to find local address\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Start listening
    if (listen(sockfd, BACKLOG) == SOCKET_ERROR)
    {
        closesocket(sockfd);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return sockfd;
}

