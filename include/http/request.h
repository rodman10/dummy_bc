//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_REQUEST_H
#define BLOCK_CHAIN_REQUEST_H

#include <util/precompiled.h>
#include <http_parser.h>
#include <string.h>
#include <netdb.h>
#include <web/route.h>
#include <http/connect.h>



//TODO avoid malloc
STR HTTP_Get(CSTR url);

STR HTTP_Get(CSTR url, HTTP_Headers &headers);

//TODO more post method process
STR HTTP_Post(CSTR url, CSTR body);

STR HTTP_Post(CSTR url, HTTP_Headers &headers, CSTR body);

#endif //BLOCK_CHAIN_REQUEST_H
