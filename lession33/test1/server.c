#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(void) {
    //1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }
    //2.设置本地的通信方式IP4,端口号,以及IP地址
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
    if(result1 < 0) {
        perror("listen");
        exit(-1);
    }
    //5.接受客户端连接-需要循环的去接受，因为有不止一个客户端接入
    while(1) {
        //创建子进程去处理具体通信
        pid_t pid = fork();
        if (pid > 0){
            //父进程
            struct sockaddr_in clientLogAddr; //用于记录连接成功后客户端的地址信息
            socklen_t len = sizeof(clientLogAddr);
            int fd1 =  accept(fd,(struct sockaddr *)&clientLogAddr,&len);
            if(fd1 < 0){
                perror("accept");
                exit(-1);
            }
            printf("i am parent process, pid : %d, ppid : %d\n", getpid(), getppid());

            //输出客户端的信息
            char clientIP[16];
            inet_ntop(AF_INET, &clientLogAddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
            unsigned short clientPort = ntohs(clientLogAddr.sin_port);
            printf("client ip is %s, port is %d\n", clientIP, clientPort);
            
            //5.通信
            char recvBuf[1024] = {0};
            while(1) {
                //获取客户端的数据
                int num = read(fd1, recvBuf, sizeof(recvBuf));
                if(num == -1) {
                    perror("read");
                    exit(-1);
                } else if(num > 0) {
                    printf("recv client data : %s\n", recvBuf);
                } else if(num == 0) {
                    // 表示客户端断开连接
                    printf("clinet closed...");
                    break;
                }
                char * data = "hello,i am server";
                //给客户端发送数据
                write(fd1, data, strlen(data));
            }
            // 关闭进程的文件描述符
            close(fd1);
            close(fd);
        } else if (pid == 0){
            //子进程
            printf("i am child process, pid : %d, ppid : %d\n", getpid(),getppid());
            // 1.创建套接字
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            if(fd == -1) {
                perror("socket");
                exit(-1);
            }
            // 2.连接服务器端
            struct sockaddr_in serveraddr;
            serveraddr.sin_family = AF_INET;
            inet_pton(AF_INET, "192.168.54.128", &serveraddr.sin_addr.s_addr);
            serveraddr.sin_port = htons(4444);
            int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
            if(ret == -1) {
                perror("connect");
                exit(-1);
            }
            // 3. 通信
            char recvBuf[1024] = {0};
            while(1) {
                char * data = "hello,i am client";
                // 给客户端发送数据
                write(fd, data , strlen(data));
                sleep(1);
                int len = read(fd, recvBuf, sizeof(recvBuf));
                if(len == -1) {
                    perror("read");
                    exit(-1);
                } else if(len > 0) {
                    printf("recv server data : %s\n", recvBuf);
                } else if(len == 0) {
                    // 表示服务器端断开连接
                    printf("server closed...");
                    break;
                }
            }
            // 关闭连接
            close(fd);
        }
        close(fd);
    }
    return 0;
}