#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <exception>
#include <sys/epoll.h>
#include <signal.h>
#include "threadpool.h"
#include "http_conn.h"
#include "locker.h"

#define MAX_FD 65535    //主线程最大处理量(最大的文件描述符个数)
#define MAX_ENEVTS 10000    //最大监听数量



void addsig(int sig,void(sighandler)(int)){ //添加信号捕捉->当网络通信的一端断开连接了，另一端任然在往这一端写数据，就会触发信号，我们用addsig接收这个信号，并处理
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));    //将sa的sizeof(sa)位字节都置为'\0'
    sa.sa_handler = sighandler;
    sigfillset(&sa.sa_mask);
    // 如果需要设置信号掩码，可以使用如下语句：
    // sigemptyset(&sa.sa_mask);
    // sigaddset(&sa.sa_mask, SIGANOTHER);

    // 如果需要设置其他标志位，可以使用如下语句：
    // sa.sa_flags = SA_RESTART;
    sigaction(sig, &sa, NULL);
}

extern void addfd(int epollfd,int fd,bool one_shot);    //添加文件描述符到epoll中
extern void removefd(int epollfd,int fd);   //从epoll中删除文件描述符
extern void modfd(int epollfd,int fd,int event);    //修改文件描述符事件
extern int m_epollfd;   //epoll实例的文件描述符(静态)

int main(int argc, char const *argv[])  //主线程
{
    if(argc < 1){
        printf("illegal input\n");
        exit(-1);
    }
    int netport = atoi(argv[1]);    //获取端口号
    // int hostport = ntohs(netport);   //大小端转换
    addsig(SIGPIPE,SIG_IGN);    //信号捕捉,将捕捉到的SIGPIPE信号忽略掉，程序将继续执行（后续也可以补充处理，而不是简单忽略）
    
    threadpool<http_conn>* pool = NULL; //初始化线程池
    try{
        pool = new threadpool<http_conn>();
    }catch(...){
        exit(-1);
    }
    http_conn* users = new http_conn[ MAX_FD ]; //创建一个数组用于保存所有的客户端信息
    int lfd = socket(PF_INET, SOCK_STREAM, 0);  // 创建socket
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in saddr;   // 创建sockaddr 并绑定属性
    saddr.sin_port = htons(netport);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    int optval = 1; // 设置端口复用(要在绑定之前设置)
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); 
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));  // 绑定
    if(ret == -1) {
        perror("bind");
        return -1;
    }
    ret = listen(lfd, 8);   // 监听
    if(ret == -1) {
        perror("listen");
        return -1;
    }
    struct epoll_event epollEvents [ MAX_ENEVTS ];  //监听事件数组：作为传出参数的载体，保存了发生变化的文件描述符的信息
    int epfd = epoll_create(1); // IO多路复用->调用epoll_create()创建一个epoll实例
    addfd(epfd,lfd,false);   //将lfd添加到epoll中（监听事件不需要设置one_shot）
    http_conn::m_epollfd = epfd;   //设置静态变量m_epollfd的值
    while(1){
        int ret = epoll_wait(epfd,epollEvents,MAX_ENEVTS,-1);   //ret：检测到了几个事件发生
        if((ret < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }
        for(int i = 0;i < ret;i++){
            int sockfd = epollEvents[i].data.fd;
            if(sockfd == lfd){  //有新客户端接入
                struct sockaddr_in caddr;//记录接入socket的客户端信息
                socklen_t len = sizeof(caddr);
                int connlfd = accept(lfd,(struct sockaddr *)&caddr,&len);  //clfd是某个客户端接入lfd的socket后为它创建的用于通信的socket_fd
                if (connlfd == -1) {
                    perror("accept");
                    continue;
                }
                if(http_conn::m_users_count >= MAX_FD){
                    //给客户端写一个信息告诉它服务器这百年接入的客户端已经满了，请稍等
                    close(connlfd);
                    continue;
                }
                users[sockfd].init(connlfd,caddr);
            }else if(epollEvents[i].events & (EPOLLIN)){
                if(users[sockfd].read()){
                    //一次性把所有数据都读完
                    pool->append(users + sockfd);
                }else{
                    users[sockfd].close_conn();
                }
            }else if(epollEvents[i].events & (EPOLLOUT)){
                if( !users[sockfd].write()){
                    users[sockfd].close_conn();
                }
            }else if(epollEvents[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                //发生异常事件->关闭连接
                users[sockfd].close_conn();  
            }
        }
    }
        close(epfd);    
        close(lfd);
        delete [] users;    //释放请求数组new出的内存
        delete pool;    //释放池new出的内存
    return 0;
}
