#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
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
    
    //初始化 标志位集合
    fd_set reads,temp;
    FD_ZERO(&reads);
    FD_ZERO(&temp);
    FD_SET(lfd,&reads);
    //将被监听到的文件描述符记录下来
    int max = lfd;

    //无限循环select
    while(1) {
        sleep(1);
        temp = reads;
        //调用select系统函数，让内核帮检测哪些文件描述符有数据
        //每次有客户端接入就将接入客户端的文件描述符录入标志位
        int ret = select(max+1,&temp,NULL,NULL,NULL);    
        if(ret == -1) {
            perror("select");
            exit(-1);
        }else if(ret == 0) {
            continue;
        }else if(ret > 0) {
            //说明检测到了有文件描述符的对应的缓冲区的数据发生了改变
            //文件描述符标志位集合有变动，说明有客户端发送数据进来，
            //返回个数：有那几个客户端发送了数据进来 
            if(FD_ISSET(lfd, &temp)) {
                // 表示有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                // 将新的文件描述符加入到集合中
                FD_SET(cfd, &reads);

                // 更新最大的文件描述符
                max = max > cfd ? max : cfd;
            }
            for(int i = lfd + 1; i <= max; i++) {
                if(FD_ISSET(i, &temp)) {
                    // 说明这个文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(i, buf, sizeof(buf));
                    if(len == -1) {
                        perror("read");
                        exit(-1);
                    } else if(len == 0) {
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i, &reads);
                    } else if(len > 0) {
                        printf("read buf = %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
            }
        }
    }
}
    close(lfd);
    return 0;
}