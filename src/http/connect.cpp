//
// Created by huang on 19-6-9.
//

#include <connect.h>
#include <http_parser.h>
#include <http/http.h>

int InitConnection(CSTR hostname, CSTR port, struct addrinfo **res) {
    struct addrinfo hints;
    memset(&hints, 0 , sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (strlen(port)) {
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

char * buildRequest(CSTR hostname, CSTR path, int method, CSTR (*headers)[2], int hnum, CSTR body) {
    const char *method_str;
    switch (method) {
        bcase 1:
            method_str = "GET";
        bcase 1<<1:
            method_str = "POST";
        bdefault:
            return nullptr;
    }
    int header_len = 0, body_len = STRLEN(body);
    for (int i = 0; i < hnum; ++i) {
        // why plus 4: space, :, \r, \n
        header_len += strlen(headers[i][0]) + strlen(headers[i][1]) + 4;
    }
    // why plus 2: between header and body
    char *buf = malloc_str(BASIC_REQUEST_STR_LEN(method_str, path, hostname) + header_len + 2 + body_len);
    BASIC_REQUEST_STR(buf, method_str, path, hostname);

    for (int i = 0; i < hnum; ++i) {
        sprintf(buf,"%s%s: %s\r\n", buf, headers[i][0], headers[i][1]);
    }
    sprintf(buf, "%s\r\n", buf);
    if (body_len) {
        sprintf(buf, "%s%s", buf, body);
    }
    return buf;
}

int MakeRequest(int sockfd, CSTR hostname, CSTR path, int method, CSTR (*headers)[2], int hnum, CSTR body) {
    int bytes_sent;
    char *request = buildRequest(hostname, path, method, headers, hnum, body);
    printf("Bytes to send: %ld\n", STRLEN(request));
    bytes_sent = send(sockfd, request, STRLEN(request), 0);
    free_ptr(request);
    if (bytes_sent==-1) {
        return -1;
    }
    shutdown(sockfd, SHUT_WR);
    return bytes_sent;
}



int FetchResponse(int sockfd, STR *response) {
    int bytes_received, tot_bytes_received = 0;
    char buf[MAXLINE];
    while ((bytes_received = recv(sockfd, buf, MAXLINE, 0))>0) {
        int len = tot_bytes_received + bytes_received;
        *response = (char *)realloc(*response, (len+1)* sizeof(char));
        copy_str(*response+tot_bytes_received, buf, bytes_received);
        tot_bytes_received = len;
    }
    close(sockfd);
    if (bytes_received == -1) {
        return -1;
    }
    return tot_bytes_received;
}