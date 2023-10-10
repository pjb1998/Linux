/*
    TCP通信实现-创建服务器端
*/
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/socket.h>
#include <unistd.h>
#include <string.h>

int main(void) {

    //1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    //2.设置本地的 IP 地址和端口号
    struct sockaddr_in localHostAddr;
    localHostAddr.sin_family = AF_INET;
    localHostAddr.sin_port = htons(4444);
    //IP地址的转换
    //点分十进制的字符串形式->int形式
    const char * stringIP = "192.168.54.128";
    inet_pton(AF_INET,stringIP,&localHostAddr.sin_addr.s_addr);

    //3.绑定 IP 地址和端口号
    int result = bind(fd,(struct sockaddr *)&localHostAddr,sizeof(localHostAddr));
    if(result < 0) {
        perror("bind");
        exit(-1);
    }

    //4.开启监听
    int result1 = listen(fd,8);
    if(result1 < 0){
        perror("listen");
        exit(-1);
    }

    //5.接受客户端连接
    struct sockaddr_in clientLogAddr; //用于记录连接成功后客户端的地址信息
    socklen_t len = sizeof(clientLogAddr);
    int fd1 =  accept(fd,(struct sockaddr *)&clientLogAddr,&len);
    if(fd1 < 0){
        perror("accept");
        exit(-1);
    }

    //打印已连接的客户端信息

    //1.将网络字节序的整数IP，转换成点分十进制的IP地址字符串并打印
    char clientIP[16];
    inet_ntop(AF_INET,&(clientLogAddr.sin_addr),clientIP,sizeof(clientIP));
    printf("client ip = %s",clientIP);

    //2.将网络字节序的端口，转换成主机字节序的端口并打印
    unsigned short port;
    port = ntohs(clientLogAddr.sin_port);
    printf("client port = %hu\n ",port);
    char recvBuf[1024] = {0};

    //6.获取客户端的数据
    int readResult = read(fd1,recvBuf,sizeof(recvBuf));
    if(readResult == -1){
        perror("read");
        exit(-1);
    } else if(readResult == 0){
        //客户端断开连接
        printf("客户端断开连接....\n");
    } else if(readResult > 0){
        //读到客户端的数据
        printf("recv client data : %s\n", recvBuf);
    }

    //7.给数据端发送数据
    char * sendToClient = "hello,i am server";
    int writeResult = write(fd1,sendToClient,strlen(sendToClient));
    if(writeResult == -1){
        perror("write");
        exit(-1);
    }
       
    // 关闭文件描述符
    close(fd);
    close(fd1);

    return 0;
}