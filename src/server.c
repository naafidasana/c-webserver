/**
 * Simple HTTP server written in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "../headers/net.h"
#include "../headers/file.h"
#include "../headers/cache.h"
#include "../headers/mime.h"

#pragma comment(lib, "Ws2_32.lib")


#define PORT "3490"
#define REQUEST_BUFFER_SIZE 65536
#define MAX_RESPONSE_SIZE 262144

#define SERVER_FILES "../serverfiles/"
#define SERVER_ROOT "../serverroot"


void perror(const char* s)
{
    fprintf(stderr, "%s: %d\n", s, WSAGetLastError());
}


/**
 * Send back response
 */
int send_response(SOCKET fd, char* header, char* content_type, void* body, int content_length)
{
    char response[MAX_RESPONSE_SIZE];
    int response_length = snprintf(
        response, MAX_RESPONSE_SIZE,
        "%s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n",
        header, content_type, content_length
    );

    // Ensure there is enough space in the response buffer for the boyd
    if (response_length + content_length >= MAX_RESPONSE_SIZE)
    {
        fprintf(stderr, "Response body is too large to fit in the buffer\n");
        return -1;
    }

    memcpy(response + response_length, body, content_length);
    response_length += content_length;

    int rv = send(fd, response, response_length, 0);

    if (rv < 0)
        perror("send");

    return rv;
}

/**
 * Send back random number
 * Allows for quickly testing server
 */
void get_d20(SOCKET fd)
{
    srand(time(NULL));
    int random_num = (rand() % 20) + 1;
    char response_body[8];
    snprintf(response_body, sizeof(response_body), "%d\n", random_num);
    send_response(fd, "HTTP/1.1 200 OK", "text/plain", response_body, strlen(response_body));
}


/**
 * Default response for when requested resource cannot be found
 */
void resp_404(SOCKET fd)
{
    char filepath[4096];
    file_data* filedata;
    char* mime_type;

    snprintf(filepath, sizeof(filepath), "%s/404.html", SERVER_FILES);
    filedata = load_file(filepath);

    if (filedata == NULL)
    {
        fprintf(stderr, "cannot find system 404 file\n");
        return;
    }

    mime_type = get_mime_type(filepath);
    send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);
    printf("Mime Type: %s", mime_type);
    free_file(filedata);
}

/**
 * Handle get request
 */
void handle_get_request(SOCKET fd, Cache* cache, char* path)
{
    // Remove the leading `/` from the path and prepend to the SERVER_FILES dir
    char filepath[2048];
    snprintf(filepath, sizeof(filepath), "%s%s", SERVER_FILES, path + 1);

    // Check that the requested file exists
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1)
    {
        // Send 404 response
        resp_404(fd);
        return;
    }

    // Load the file from disk. [TO DO: Check if file is already in Cache and load from there]
    file_data* fdata = load_file(filepath);
    if (fdata == NULL)
    {
        resp_404(fd);
        return;
    }

    // Get MIME type
    char* mime_type = get_mime_type(filepath);

    // Send the response
    send_response(fd, "HTTP/1.1 200 OK", mime_type, fdata->data, fdata->size);

    // Free the loaded file
    free_file(fdata);
}

/**
 * Handle HTTP request
 */
void handle_http_request(SOCKET fd, Cache* cache)
{
    char request[REQUEST_BUFFER_SIZE];
    
    int bytes_recvd = recv(fd, request, REQUEST_BUFFER_SIZE - 1, 0);

    if (bytes_recvd < 0)
    {
        perror("recv");
        return;
    }

    request[bytes_recvd] = '\0';

    char method[8], path[1024];
    sscanf(request, "%s %s", method, path);

    if (strcmp(method, "GET") == 0)
    {
        if (strcmp(path, "/d20") == 0)
            get_d20(fd);
        else
        {
        // Get file (from cache or disk). [Implement later]
        handle_get_request(fd, cache, path);
        }
    }
    else 
    {
        // No support for other method yet. Simply return 404
        resp_404(fd);
    }    
}


int main()
{
    WSADATA wsaData;
    int rv;
    SOCKET listenfd, newfd;
    struct sockaddr_storage their_addr;
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)&their_addr;
    int addr_len = sizeof(their_addr);

    Cache* cache = create_cache(10, 1);


    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
        exit(1);
    }

    // Get a listening socket
    listenfd = get_listener_socket(PORT);
    if (listenfd == INVALID_SOCKET)
    {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        WSACleanup();
        exit(1);
    }

    printf("webserver: waiting for connection on port %s...\n", PORT);

    // Main server loop
    while(1)
    {
        newfd = accept(listenfd, (struct sockaddr*)&their_addr, &addr_len);
        if (newfd == INVALID_SOCKET)
        {
            fprintf(stderr, "accept failed: %d\n", WSAGetLastError());
            continue;
        }

        
        inet_ntoa(ipv4->sin_addr);

        printf("server: got connection from %s\n", s);

        handle_http_request(newfd, cache);

        closesocket(newfd);
    }

    // Cleanup
    closesocket(listenfd);
    WSACleanup();

    return 0;
}
