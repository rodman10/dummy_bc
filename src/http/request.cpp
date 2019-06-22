//
// Created by huang on 19-6-9.
//

#include <http/connect.h>
#include <http/request.h>

int HTTP_Get(CSTR url, HTTP_Response& response) {
    HTTP_Headers headers;
    HTTP_Get(url, headers, response);
}


int HTTP_Get(CSTR url, HTTP_Headers &headers, HTTP_Response& response) {
    int sockfd;
    addrinfo *res;
    HTTP_Request request(url, headers);
    request.method = "GET";
    InitConnection(request.hostname, request.port, &res);
    if ((sockfd = MakeConnection(res)) == -1) {
        return -1;
    }
    if (MakeRequest(sockfd, request) == -1) {
        return -1;
    }
    return FetchResponse(sockfd, response);
}

int HTTP_Post(CSTR url, CSTR body, HTTP_Response& response) {
    HTTP_Headers headers;
    HTTP_Post(url, headers, body, response);
}

//TODO more post method process
int HTTP_Post(CSTR url, HTTP_Headers &headers, CSTR body, HTTP_Response& response) {
    int sockfd;
    addrinfo *res;
    HTTP_Request request(url, headers);
    request.SetBody(body, strlen(body));
    request.method = "POST";
    InitConnection(request.hostname, request.port, &res);
    if ((sockfd = MakeConnection(res))==-1) {
        return -1;
    }
    if (MakeRequest(sockfd, request) == -1) {
        return -1;
    }
    return FetchResponse(sockfd, response);
}