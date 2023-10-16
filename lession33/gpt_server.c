#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    portno = 4545;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Error accepting connection");
        exit(1);
    }

    memset(buffer, 0, sizeof(buffer));
    n = read(newsockfd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    printf("Received request:\n%s\n", buffer);

    // 构造 JSON 数据
    char response[1024];
    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"message\": \"Hello, world!\"}");

    // 发送 JSON 数据到浏览器
    n = write(newsockfd, response, strlen(response));
    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}