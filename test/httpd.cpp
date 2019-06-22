//
// Created by huang on 19-6-7.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <http/http_parser.h>
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
    HTTP_Request request;
    HTTP_Parser<HTTP_Request> parser(&request);
    parser.Parse(client);
    auto func = Route(request.path, HttpStr2HttpEnum(request.method));
    STR data = nullptr;
    if (func) {
        auto &&res = func(request);
        data = res.Serialize();
    } else {
        HTTP_Response res("Invalid method", TEXT_PLAIN, HTTP_STATUS_BAD_REQUEST);
        data = res.Serialize();
    }

    size_t data_len = STRLEN(data);
    size_t bytes_sent, tot_bytes_sent = 0;
    while (tot_bytes_sent < data_len) {
        bytes_sent = send(client, data + tot_bytes_sent, 1024, 0);
        if (bytes_sent == -1) {
            goto end;
        }
        tot_bytes_sent += bytes_sent;
    }
    end:
    FREE(data);
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
int main(int argc, char **argv) {
    block_chain.CreateGenesisBlock();

    int server_sock, client_sock;
    int port = atoi(argv[1]);
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
