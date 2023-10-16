/*
    多线程实现并发服务器
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//
struct sockInfo {
    int fd; // 通信的文件描述符
    struct sockaddr_in addr;
    pthread_t tid;  // 线程号
};

struct sockInfo sockInfoArr [128];

int main(void) {
    //1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }
    //2.添加sockaddr地址
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    //3.绑定
    int ret = bind(fd,(struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }
    //4.开启监听
    int result1 = listen(fd,8);
    if(result1 < 0) {
        perror("listen");
        exit(-1);
    }
    //5.
    
    return 0;
}