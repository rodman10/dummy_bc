//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_CONNECT_H
#define BLOCK_CHAIN_CONNECT_H

#include <util/precompiled.h>
#include <netdb.h>
#include <http/http.h>


int InitConnection(CSTR hostname, CSTR port, struct addrinfo **res);

int MakeConnection(struct addrinfo *res);

int MakeRequest(int sockfd, HTTP_Request &request);

int FetchResponse(int sockfd, STR *response);

#endif //BLOCK_CHAIN_CONNECT_H
