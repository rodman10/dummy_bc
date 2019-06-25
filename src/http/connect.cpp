//
// Created by huang on 19-6-9.
//
#include <algorithm>
#include <connect.h>
#include <http/http_parser.h>

int InitConnection(CSTR hostname, CSTR port, struct addrinfo **res) {
    struct addrinfo hints;
    memset(&hints, 0 , sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (STRLEN(port)) {
        return getaddrinfo(hostname, port, &hints, res);
    } else {
        return getaddrinfo(hostname, "80", &hints, res);
    }
}

int MakeConnection(struct addrinfo *res) {
    int sockfd;
    if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol))==-1) {
        return -1;
    }
    if (connect(sockfd, res->ai_addr, res->ai_addrlen)==-1) {
        return -1;
    }
    return sockfd;
}

//char * buildRequest(CSTR hostname, CSTR path, int method, HTTP_Base &headers, CSTR body) {
//    const char *method_str;
//    switch (method) {
//        bcase 1<<HTTP_GET:
//            method_str = "GET";
//        bcase 1<<HTTP_POST:
//            method_str = "POST";
//        bdefault:
//            return nullptr;
//    }
//    int header_len = headers.headers_length, body_len = STRLEN(body);
//    // why plus 2: between header and data
//    char *buf = MALLOC_STR(BASIC_REQUEST_STR_LEN(method_str, path) + header_len + BLANK_LINE + body_len);
//    BASIC_REQUEST_STR(buf, method_str, path);
//
//    for (int i = 0; i < headers.headers_size; ++i) {
//        sprintf(buf,"%s%s: %s\r\n", buf, headers.headers[i][0], headers.headers[i][1]);
//    }
//    if (body_len) {
//        sprintf(buf, "%s\r\n%s", buf, body);
//    }
//    return buf;
//}

int MakeRequest(int sockfd, HTTP_Request &request) {
    int bytes_sent;
    auto data = request.Serialize();
    bytes_sent = send(sockfd, data, STRLEN(data), 0);
    FREE(data);
    if (bytes_sent==-1) {
        return -1;
    }
    shutdown(sockfd, SHUT_WR);
    return bytes_sent;
}

int FetchResponse(int sockfd, HTTP_Response& response) {
    HTTP_Parser<HTTP_Response> parser(&response);
    parser.Parse(sockfd);
    close(sockfd);
    return 0;
//
//    int bytes_received, tot_bytes_received = 0;
//    char buf[MAXLINE];
//    while ((bytes_received = recv(sockfd, buf, MAXLINE, 0))>0) {
//        int len = tot_bytes_received + bytes_received;
//        auto tmp = (char *)realloc(*response, (len+1)* sizeof(char));
//        if (!tmp) {
//            close(sockfd);
//            return -1;
//        }
//        *response = tmp;
//        COPY_STR(*response+tot_bytes_received, buf, bytes_received);
//        tot_bytes_received = len;
//    }
//    close(sockfd);
//    if (bytes_received == -1) {
//        return -1;
//    }
//    return tot_bytes_received;
}