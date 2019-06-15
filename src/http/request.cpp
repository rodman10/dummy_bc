//
// Created by huang on 19-6-9.
//

#include <http/connect.h>
#include <http_parser.h>
#include <http/request.h>

STR HTTP_Get(CSTR url, CSTR (*headers)[2], int hnum) {
    auto n = strlen(url);
    int sockfd;
    STR response = nullptr;
    PARSE_URL(parser, url, n);
    addrinfo *res;
    URL_FIELD(hostname, parser, UF_HOST);
    URL_FIELD(port, parser, UF_PORT);
    URL_FIELD(path, parser, UF_PATH);
    InitConnection(hostname, port, &res);
    if ((sockfd = MakeConnection(res))==-1) {
        return nullptr;
    }
    if (MakeRequest(sockfd, hostname, path, HTTP_GET, headers, hnum, nullptr) == -1) {
        goto end;
    }
    FetchResponse(sockfd, &response);
    end:
    free_ptr(path);
    free_ptr(port);
    free_ptr(hostname);
    return response;
}

//TODO more post method process
STR HTTP_Post(CSTR url, CSTR (*headers)[2], int hnum, CSTR body) {
    auto n = strlen(url);
    int sockfd;
    STR response = nullptr;
    PARSE_URL(parser, url, n);
    addrinfo *res;
    URL_FIELD(hostname, parser, UF_HOST);
    URL_FIELD(port, parser, UF_PORT);
    URL_FIELD(path, parser, UF_PATH);
    InitConnection(hostname, port, &res);
    if ((sockfd = MakeConnection(res))==-1) {
        return nullptr;
    }
    if (MakeRequest(sockfd, hostname, path, HTTP_POST, headers, hnum, body) == -1) {
        goto end;
    }
    FetchResponse(sockfd, &response);
    end:
    free_ptr(path);
    free_ptr(port);
    free_ptr(hostname);
    return response;
}