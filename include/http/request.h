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

#define PARSE_URL(parser, url, len) \
    auto parser = (http_parser_url *)malloc(sizeof(http_parser_url)); \
    http_parser_url_init(parser); \
    http_parser_parse_url(url, len, 0, parser);

#define URL_FIELD_OFF(parser, f) \
    (parser)->field_data[f].off

#define URL_FIELD_LEN(parser, f) \
    (parser)->field_data[f].len

#define URL_FIELD(param, parser, f) \
    STR param = malloc_str(URL_FIELD_LEN(parser, f)); \
    copy_str(param, url + URL_FIELD_OFF(parser, f), URL_FIELD_LEN(parser, f));

//TODO avoid malloc
STR HTTP_Get(CSTR url, CSTR (*headers)[2], int hnum);

//TODO more post method process
STR HTTP_Post(CSTR url, CSTR (*headers)[2], int hnum, CSTR body);

#endif //BLOCK_CHAIN_REQUEST_H
