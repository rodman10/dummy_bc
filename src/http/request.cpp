//
// Created by huang on 19-6-9.
//

#include <http/connect.h>
#include <http/request.h>

STR HTTP_Get(CSTR url) {
    HTTP_Headers headers;
    HTTP_Get(url, headers);
}


STR HTTP_Get(CSTR url, HTTP_Headers &headers) {
    int sockfd;
    STR response = nullptr;
    addrinfo *res;
    HTTP_Request request(url, headers);
    request.method = "GET";
    InitConnection(request.hostname, request.port, &res);
    if ((sockfd = MakeConnection(res))==-1) {
        return nullptr;
    }
    if (MakeRequest(sockfd, request) == -1) {
        return nullptr;
    }
    FetchResponse(sockfd, &response);
    return response;
}

STR HTTP_Post(CSTR url, CSTR body) {
    HTTP_Headers headers;
    HTTP_Post(url, headers, body);
}

//TODO more post method process
STR HTTP_Post(CSTR url, HTTP_Headers &headers, CSTR body) {
    auto n = strlen(url);
    int sockfd;
    STR response = nullptr;
    addrinfo *res;
    HTTP_Request request(url, headers);
    request.set_body(body, strlen(body));
    request.method = "POST";
    InitConnection(request.hostname, request.port, &res);
    if ((sockfd = MakeConnection(res))==-1) {
        return nullptr;
    }
    if (MakeRequest(sockfd, request) == -1) {
        return nullptr;
    }
    FetchResponse(sockfd, &response);
    return response;
}