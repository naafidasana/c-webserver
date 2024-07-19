#ifndef NET_H
#define NET_H


#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma once

void* get_in_addr(struct sockaddr* sa);
SOCKET get_listener_socket(char* port);

#endif