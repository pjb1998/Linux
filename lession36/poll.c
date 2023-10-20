#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>

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
    //初始化检测的文件描述符数组
    struct pollfd myfd [1024];
    int sizeofPollfd = sizeof(myfd)/sizeof(myfd[0]);
    for(int i = 0;i<sizeofPollfd;i++){
        myfd[i].fd = -1;
        myfd[i].events = POLLIN;
    }
    myfd[0].fd = lfd;
    int nfds = 0;

    //无限循环poll
    while(1) {
        sleep(1);
        int ret = poll(myfd,nfds+1,-1);
        if(ret == -1) {
            perror("poll");
            exit(-1);
        } else if(ret > 0) {
            if(myfd[0].revents & POLLIN) {
                // 表示有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
                for(int i = 1;i<sizeofPollfd;i++) {
                    if(myfd[i].fd == -1){
                        myfd[i].fd = cfd;
                        myfd[i].events = POLLIN;
                        break;
                    }
                }
                // 更新最大的文件描述符的索引
                nfds = nfds > cfd ? nfds : cfd;
            }
            for(int i = 1; i <= nfds; i++) {
                if(myfd[i].revents & POLLIN) {
                    // 说明这个文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(myfd[i].fd, buf, sizeof(buf));
                    if(len == -1) {
                        perror("read");
                        exit(-1);
                    } else if(len == 0) {
                        printf("client closed...\n");
                        close(myfd[i].fd);
                        myfd[i].fd = -1;
                    } else if(len > 0) {
                        printf("read buf = %s\n", buf);
                        write(myfd[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }


        }

    }
    return 0;
}