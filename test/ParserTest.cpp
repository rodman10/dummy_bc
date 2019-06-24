//
// Created by huang on 19-6-23.
//

#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#define BUFFER_SIZE 1024

int main(int argc, const char * argv[])
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_addr.sin_zero), 8);

    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1)
    {
        perror("socket error");
        return 1;
    }
    char input_msg[BUFFER_SIZE] = "POST / http/1.1\r\nHost: 127.0.0.1\r\nContent-Type: plain/text\r\n\r\n";

    if(connect(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == 0)
    {
        fd_set client_fd_set;
        struct timeval tv;

        while(1)
        {

            if(send(server_sock_fd, input_msg, strlen(input_msg), 0) == -1)
            {
                perror("发送消息出错!\n");
            }
        }
        //}
    }
    return 0;
}