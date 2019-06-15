//
// Created by huang on 19-6-7.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include <http/callback.h>
#include <block_chain/BlockChain.h>

std::unordered_set<std::string> peers;
bool InitRoute();


void error_die(const char *sc)
{
    perror(sc);
    exit(1);
}

void *accept_request(void *arg) {
    int client = (int)(intptr_t)arg;
    STR data = nullptr;
    int bytes_received, tot_bytes_received = 0, parsed_bytes = 0;
    char buf[MAXLINE];
    HTTP_Request request;
    http_parser_settings settings;
    settings.on_header_field = headerk_callback;
    settings.on_header_value = headerv_callback;
    settings.on_body = body_callback;
    settings.on_url = url_callback;
    settings.on_message_complete = message_complete;
    http_parser *parser = (http_parser *)malloc(sizeof(http_parser));
    parser->data = (void *)&request;
    http_parser_init(parser, HTTP_REQUEST);
    while (!parser->status_code && (bytes_received = recv(client, buf, MAXLINE, 0))>0) {
        int len = tot_bytes_received + bytes_received;
        data = (char *)realloc(data, (len+1)* sizeof(char));
        copy_str(data+tot_bytes_received, buf, bytes_received);
        tot_bytes_received = len;
        parsed_bytes += http_parser_execute(parser, &settings, data+parsed_bytes, bytes_received);
    }
    if (bytes_received == -1) {
        goto end;
    }
    Route(request.url, 1<<parser->method)(request);
    free(data);
    end:
    close(client);
}

int startup(int &port) {
    InitRoute();
    int on = 1;
    struct sockaddr_in server_addr;

    int httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1) {
        error_die("ERROR: create socket failed!");
    }
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on)) < 0) {
        error_die("ERROR: set sockopt failed!");
    }

    if (bind(httpd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error_die("ERROR: bind socket failed!");
    }

    if (listen(httpd, 5) < 0) {
        error_die("ERROR: listen socket failed!");
    }
    return httpd;
}

huang::BlockChain<Transaction> block_chain;

int main() {
    int server_sock, client_sock;
    int port = 5000;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    pthread_t new_thread;

    server_sock = startup(port);
    printf("httpd running on port %d\n", port);

    while (1) {
        client_sock = accept(server_sock, (
        struct sockaddr *)& client_addr, &client_addr_len);
        if (client_sock == -1) {
            error_die("ERROR: accept socket failed!");
        }

        if (pthread_create(&new_thread, nullptr, accept_request, (void *)client_sock) != 0) {
            perror("ERROR: create pthread failed!");
        }
    }

    close(server_sock);
    return 0;
}
