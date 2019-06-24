//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_REQUEST_H
#define BLOCK_CHAIN_REQUEST_H

#include <util/precompiled.h>
#include <string.h>
#include <netdb.h>
#include <web/route.h>
#include <http/connect.h>



//TODO avoid malloc
int HTTP_Get(CSTR url, HTTP_Response& response);

int HTTP_Get(CSTR url, HTTP_Headers &headers, HTTP_Response& response);

//TODO more post method process
int HTTP_Post(CSTR url, CSTR body, HTTP_Response& response);

int HTTP_Post(CSTR url, HTTP_Headers &headers, CSTR body, HTTP_Response& response);

#endif //BLOCK_CHAIN_REQUEST_H
