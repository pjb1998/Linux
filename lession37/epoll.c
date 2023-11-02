#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if(lfd == -1) {
        perror("socket");
        return -1;
    }
    // 创建sockaddr 并绑定属性
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(6666);
    // 设置端口复用
    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    // 绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        return -1;
    }
    // 监听
    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
        return -1;
    }
    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(2000);

    struct epoll_event epollEvent;
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = lfd;

    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epollEvent);

    struct epoll_event epollEvents [1024];  //作为传出参数的载体，保存了发生变化的文件描述符的信息

    while(1) {
        sleep(1);
        int ret = epoll_wait(epfd, epollEvents, sizeof(epollEvents)/sizeof(epollEvents[0]), -1);    //函数将返回发生事件的数量
        if(ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }
        printf("ret = %d\n", ret);
            for(int i = 0; i < ret; i++) {
                int curfd = epollEvents[i].data.fd;
                if(curfd == lfd) {
                    // 监听的文件描述符有数据达到，有客户端连接
                    struct sockaddr_in cliaddr;
                    int len = sizeof(cliaddr);
                    /*
                        accept()接受一个客户端的连接请求，并返回一个新的套接字（被动监听客户端的三次握手连接请求，三次握手成功即建立连接成功）。
                        所谓“新的”就是说这个套接字与之前socket()返回的用于“监听”和“接受客户端的连接请求”的套接字不是同一个套接字。
                        与本次接受的客户端之间通信是通过在这个新的套接字上发送和接收数据来完成的。既这个套接字是用于数据传输的的。
                    */
                    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                    epollEvent.events = EPOLLIN;
                    epollEvent.data.fd = cfd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epollEvent);
                } else {
                    if(epollEvents[i].events & EPOLLOUT) {
                        continue;
                    } 
                    // 有数据到达，需要通信
                    char buf[1024] = {0};
                    int len = read(curfd, buf, sizeof(buf));
                    if(len == -1) {
                        perror("read");
                        exit(-1);
                    } else if(len == 0) {
                        printf("client closed...\n");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                        close(curfd);
                    } else if(len > 0) {
                        printf("read buf = %s\n", buf);
                        write(curfd, buf, strlen(buf) + 1);
                    }
                }
            }
    }
    close(lfd);
    close(epfd);
    return 0;
}