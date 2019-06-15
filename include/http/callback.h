//
// Created by huang on 19-6-8.
//

#ifndef BLOCK_CHAIN_HANDLER_H
#define BLOCK_CHAIN_HANDLER_H

#include <http/http.h>
#include <web/route.h>
#include "connect.h"

int headerk_callback(http_parser *parser, const char *at, size_t length) {
    auto request = (HTTP_Request *)parser->data;
    request->headers = (char* (*)[2])realloc(request->headers, (1+request->hnum)* sizeof(request->headers[0]));
    request->headers[request->hnum][0] = malloc_str(length);
    copy_str(request->headers[request->hnum][0], at, length);

    return 0;
}

int headerv_callback(http_parser *parser, const char *at, size_t length) {
    auto request = (HTTP_Request *)parser->data;
    request->headers[request->hnum][1] = malloc_str(length);
    copy_str(request->headers[request->hnum][1], at, length);
    ++request->hnum;
    return 0;
}

int url_callback(http_parser *parser, const char *at, size_t length) {
    auto request = (HTTP_Request *)parser->data;
    request->url = malloc_str(length);
    copy_str(request->url, at, length);
    return 0;
}

int body_callback(http_parser *parser, const char *at, size_t length) {
    auto *request = (HTTP_Request *)parser->data;
    request->body = malloc_str(length);
    copy_str(request->body, at, length);
    return 0;
}

int message_complete(http_parser *parser) {
    parser->status_code=1;
}

#endif //BLOCK_CHAIN_HANDLER_H
