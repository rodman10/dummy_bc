//
// Created by huang on 19-6-8.
//

#ifndef BLOCK_CHAIN_HTTP_H
#define BLOCK_CHAIN_HTTP_H

#include <util/precompiled.h>
#include <http_parser.h>

#define BLANK_LINE 2

#define SERVER_STRING "Server: httpd/0.1.0\r\n"
#define BASIC_REQUEST_STR_LEN(method, path) \
    strlen("  HTTP/1.1\r\n") + strlen(method) + STRLEN(path) + 1

#define BASIC_REQUEST_STR(buf, method, path) \
    sprintf(buf, "%s %s HTTP/1.1\r\n", method, STRLEN(path) ? (path) : "/"); \

#define BASIC_RESPONSE_STR_LEN(code) \
    strlen("HTTP/1.1  \r\n") + num_len(code) + strlen(http_status_str(code))

#define BASIC_RESPONSE_STR(buf, code) \
    sprintf(buf, "HTTP/1.1 %d %s\r\n", \
            code, http_status_str(code));

#define SERIALIZE_HEADERS(buf) \
    for (int i = 0; i < headers_size; ++i) { \
        sprintf(buf,"%s%s: %s\r\n", buf, headers[i][0], headers[i][1]); \
    } \

#define SERIALIZE_BODY(buf) \
    if (body_length) { \
        sprintf(buf, "%s\r\n%s", buf, body); \
    } \

// use hash to find header
#define GET_HEADER(request, name, dest) \
    STR dest; \
    for (int i = 0; i < request.headers_size; i++) { \
        if (!strcmp(request.headers[i][0], #name)) { \
            dest = request.headers[i][1]; \
            break; \
        } \
    } \

#define ADD_HEADER(name, value) \
    AddHeaderKey(get_header_name(name), get_header_name_length(name)); \
    AddHeaderValue(get_header_value(value), get_header_value_length(value)); \

#define ADD_HEADER_WITH_OBJ(obj, name, value) \
    obj.AddHeaderKey(get_header_name(name), get_header_name_length(name)); \
    obj.AddHeaderValue(get_header_value(value), get_header_value_length(value)); \


#define HEADER_NAME_MAP(XX) \
    XX(CONTENT_TYPE, Content-Type) \
    XX(CONTENT_LENGTH, Content-Length) \
    XX(CONNECTION, Connection) \
    XX(HOST, Host) \

enum http_header_name {
#define XX(name, string) name,
    HEADER_NAME_MAP(XX)
#undef XX
};

CSTR get_header_name(http_header_name header);

size_t get_header_name_length(http_header_name header);

#define HEADER_VALUE_MAP(XX) \
    XX(APPLICATION_JSON, application/json) \
    XX(TEXT_PLAIN, text/plain) \
    XX(CLOSE, close) \
    XX(KEEP_ALIVE, keep-alive) \

enum http_header_value {
#define XX(name, string) name,
    HEADER_VALUE_MAP(XX)
#undef XX
};

CSTR get_header_value(http_header_value header);

size_t get_header_value_length(http_header_value header);

#define PARSE_URL(parser, url, len) \
    auto parser = (http_parser_url *)malloc(sizeof(http_parser_url)); \
    http_parser_url_init(parser); \
    http_parser_parse_url(url, len, 0, parser);

#define URL_FIELD_OFF(parser, f) \
    (parser)->field_data[f].off

#define URL_FIELD_LEN(parser, f) \
    (parser)->field_data[f].len

#define URL_FIELD(param, url, parser, f) \
    STR param = malloc_str(URL_FIELD_LEN(parser, f)); \
    copy_str(param, url + URL_FIELD_OFF(parser, f), URL_FIELD_LEN(parser, f));

struct HTTP_Headers {
    size_t headers_size=0;
    STR (*headers)[2]= nullptr;
    int headers_length=0;

    void AddHeaderKey(CSTR at, size_t length) {
        auto tmp = (char* (*)[2])realloc(headers, (1+headers_size)* sizeof(headers[0]));
        // TODO realloc failure
        if (!tmp) {
            return;
        }
        headers = tmp;
        headers[headers_size][0] = malloc_str(length);
        copy_str(headers[headers_size][0], at, length);
        // why plus 2: ':' and space
        headers_length += length + 2;
    }
    void AddHeaderValue(CSTR at, size_t length) {
        headers[headers_size][1] = malloc_str(length);
        copy_str(headers[headers_size][1], at, length);
        // why plus 2: \r\n
        headers_length += length + 2;
        ++headers_size;
    }

    HTTP_Headers() {}

    HTTP_Headers(const HTTP_Headers &_headers) {
        headers_length = _headers.headers_length;
        headers = _headers.headers;
        headers_size = _headers.headers_size;
    }

    virtual ~HTTP_Headers() {
        if (headers) {
            for (int i = 0; i < headers_size; ++i) {
                free(headers[i][0]);
                headers[i][0] = nullptr;
                free(headers[i][1]);
                headers[i][1] = nullptr;
            }
            FREE(headers);
        }
    }
};

struct HTTP_Base : HTTP_Headers {

    STR body;
    size_t body_length;

    HTTP_Base() {}

    HTTP_Base(HTTP_Headers &headers) : HTTP_Headers(headers) {}

    ~HTTP_Base() {

        FREE(body);
    }

    virtual STR Serialize() const = 0;
};

struct HTTP_Request: HTTP_Base {
    STR hostname;
    STR port;
    STR path;
    STR method;

    void set_hostname(CSTR at, size_t length) {
        hostname = malloc_str(length);
        copy_str(hostname, at, length);
    }

    void set_port(CSTR at, size_t length) {
        port = malloc_str(length);
        copy_str(port, at, length);
    }

    void set_path(CSTR at, size_t length) {
        path = malloc_str(length);
        copy_str(path, at, length);
    }

    void set_body(CSTR at, size_t length) {
        body = malloc_str(length);
        copy_str(body, at, length);
    }

    HTTP_Request(CSTR url, HTTP_Headers &headers) : HTTP_Base(headers) {
        auto length = strlen(url);
        PARSE_URL(parser, url, length);
        URL_FIELD(hostname, url, parser, UF_HOST);
        URL_FIELD(port, url, parser, UF_PORT);
        URL_FIELD(path, url, parser, UF_PATH);
        AddHeaderKey(get_header_name(HOST), get_header_name_length(HOST));
        AddHeaderValue(url + parser->field_data[UF_HOST].off, parser->field_data[UF_HOST].len + parser->field_data[UF_PORT].len+1);
        FREE(parser);
    }

    HTTP_Request() {}

    STR Serialize() const {
        // why plus 2: between header and body
        STR buf = malloc_str(BASIC_REQUEST_STR_LEN(method, path) + headers_length + BLANK_LINE + body_length);
        BASIC_REQUEST_STR(buf, method, path);

        SERIALIZE_HEADERS(buf);

        SERIALIZE_BODY(buf);

        return buf;
    }

    // shallow copy, RVO trick
    ~HTTP_Request() {
        FREE(hostname);
        FREE(path);
        FREE(port);
    }
};


//TODO add keep alive
struct HTTP_Response : HTTP_Base {
    http_status code;

    HTTP_Response(http_status code) : code(code) {}

    HTTP_Response(CSTR body, http_header_value body_type, http_status code) : code(code) {
        headers = nullptr;
        auto len = STRLEN(body);
        body_length = len;
        if (len) {
            this->body = malloc_str(len);
            copy_str(this->body, body, len);
        }
        // maybe add keep alive
        ADD_HEADER(CONNECTION, CLOSE);
        ADD_HEADER(CONTENT_TYPE, body_type);
        AddHeaderKey(get_header_name(CONTENT_LENGTH), get_header_name_length(CONTENT_LENGTH));
        auto n_len = num_len(len);
        STR buf = malloc_str(n_len);
        sprintf(buf, "%zu", len);
        AddHeaderValue(buf, n_len);
        FREE(buf);
    }

    STR Serialize() const {
        STR buf = malloc_str(BASIC_RESPONSE_STR_LEN(code) + headers_length + BLANK_LINE + body_length);
        BASIC_RESPONSE_STR(buf, code);

        SERIALIZE_HEADERS(buf);

        SERIALIZE_BODY(buf);

        return buf;
    }

    ~HTTP_Response() {

    }
};

#endif //BLOCK_CHAIN_HTTP_H
