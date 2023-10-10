/*
    TCP通信实现-创建服务端
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(void){
    
    //1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }
    
    //2.连接服务端
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4444);
    const char * serverIP = "192.168.54.128";
    inet_pton(AF_INET,serverIP,&serverAddr.sin_addr.s_addr);
    
    //3.建立连接
    int ret =  connect(fd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    //4.给服务端发送数据
    char * sendToServer = "hello,i am client";
    int writeResult = write(fd,sendToServer,strlen(sendToServer));
    if(writeResult == -1){
        perror("write");
        exit(-1);
    }

    //5.从服务端获取数据
    char recvBuf[1024] = {0};
    int readResult = read(fd,recvBuf,sizeof(recvBuf));
    if(readResult == -1){
        perror("read");
        exit(-1);
    } else if(readResult == 0){
        //与服务端断开连接
        printf("服务端断开连接....\n");
    } else if(readResult > 0){
        //读到服务端的数据
        printf("recv server data : %s\n", recvBuf);
    }
    // 关闭连接
    close(fd);
    return 0;
}