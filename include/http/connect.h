//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_CONNECT_H
#define BLOCK_CHAIN_CONNECT_H

#include <util/precompiled.h>
#include <netdb.h>

#define SERVER_STRING "Server: httpd/0.1.0\r\n"
#define BASIC_REQUEST_STR_LEN(method, path, hostname) \
    strlen("  HTTP/1.1\r\n")+strlen("Host: \r\n")+strlen(method)+strlen(path)+strlen(hostname)

#define BASIC_REQUEST_STR(buf, method, path, hostname) \
    sprintf(buf, "%s %s HTTP/1.1\r\n", method, STRLEN(path) ? path : "/"); \
    sprintf(buf, "%sHost: %s\r\n", buf, hostname)



int InitConnection(CSTR hostname, CSTR port, struct addrinfo **res);

int MakeConnection(struct addrinfo *res);

int MakeRequest(int sockfd, CSTR hostname, CSTR path, int method, CSTR (*headers)[2], int hnum, CSTR body);

int FetchResponse(int sockfd, STR *response);

#endif //BLOCK_CHAIN_CONNECT_H
