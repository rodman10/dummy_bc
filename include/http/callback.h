//
// Created by huang on 19-6-8.
//

#ifndef BLOCK_CHAIN_HANDLER_H
#define BLOCK_CHAIN_HANDLER_H

#include <http/http.h>
#include <web/route.h>
#include "connect.h"

int header_key_callback(http_parser *parser, const char *at, size_t length) {
    auto request = (HTTP_Request *)parser->data;
    request->AddHeaderKey(at, length);
    return 0;
}

int header_val_callback(http_parser *parser, const char *at, size_t length) {
    auto request = (HTTP_Request *)parser->data;
    request->AddHeaderValue(at, length);
    return 0;
}

int url_callback(http_parser *parser, const char *at, size_t length) {
    auto request = (HTTP_Request *)parser->data;
    request->set_path(at, length);
    return 0;
}

int body_callback(http_parser *parser, const char *at, size_t length) {
    auto *request = (HTTP_Request *)parser->data;
    request->set_body(at, length);
    return 0;
}

int message_complete(http_parser *parser) {
    parser->status_code=1;
}

#endif //BLOCK_CHAIN_HANDLER_H
