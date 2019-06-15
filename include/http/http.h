//
// Created by huang on 19-6-8.
//

#ifndef BLOCK_CHAIN_HTTP_H
#define BLOCK_CHAIN_HTTP_H

#include <util/precompiled.h>
#include <http_parser.h>

// semi RAII
struct HTTP_Request {
    char* url;
    char * (*headers)[2];
    int hnum;
    char *body;
    ~HTTP_Request() {
        free(url);
        url = nullptr;
        for (int i = 0; i < hnum; ++i) {
            free(headers[i][0]);
            headers[i][0] = nullptr;
            free(headers[i][1]);
            headers[i][1] = nullptr;
        }
        free(headers);
        headers = nullptr;
        if (nullptr != body) {
            free(body);
            body = nullptr;
        }
    }
};

struct HTTP_Response {
    char *body;
    http_status code;

    HTTP_Response(const char *body, http_status code) : code(code) {
        auto content_length = (body != nullptr) ? strlen(body) : 0;
        this->body = malloc_str(256 + content_length * sizeof(char));

        sprintf(this->body, "HTTP/1.1 %d %s\r\n\
            Connection: close\r\n\
            Content-Type: text/plain\r\n\
            Content-Length: %zu\r\n\r\n%s",
                code, http_status_str(code), content_length, body);
    }

    ~HTTP_Response() {
        if (body) {
            free(body);
            body = nullptr;
        }
    }
};

#endif //BLOCK_CHAIN_HTTP_H
