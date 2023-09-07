#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*
    #include <signal.h>
    typedef void (*sighandler_t)(int);
    sighandler_t signal(int signum, sighandler_t handler);
        - 功能：设置某个信号的捕捉行为
        - 参数：
            - signum: 要捕捉的信号
            - handler: 捕捉到信号要如何处理
                - SIG_IGN ： 忽略信号
                - SIG_DFL ： 使用信号默认的行为
                - 回调函数 :  这个函数是内核调用，程序员只负责写，捕捉到信号后如何去处理信号。
                回调函数：
                    - 需要程序员实现，提前准备好的，函数的类型根据实际需求，看函数指针的定义
                    - 不是程序员调用，而是当信号产生，由内核调用
                    - 函数指针是实现回调的手段，函数实现之后，将函数名放到函数指针的位置就可以了。

        - 返回值：
            成功，返回上一次注册的信号处理函数的地址。第一次调用返回NULL
            失败，返回SIG_ERR，设置错误号
            
    SIGKILL SIGSTOP不能被捕捉，不能被忽略！！！！！！！！
*/
void sighandler_t(int c) {
    printf("catch the signal：%d\n",c);
}

int main(void) {
    //注册信号捕捉
    signal(SIGALRM,sighandler_t);    
    //创建时间组成成分的结构体1
    struct timeval time1;
    time1.tv_sec = 3;    //3s
    time1.tv_usec = 0;   //0us
    //创建时间组成成分的结构体2
    struct timeval time2;
    time2.tv_sec = 2;    //2s
    time2.tv_usec = 0;   //0us
    //创建定时器结构体
    struct itimerval itim;
    itim.it_interval = time2;
    itim.it_value = time1;
    int ret = 0;
    ret = setitimer(ITIMER_REAL,&itim,NULL);    //非阻塞的
    printf("定时器开始了...\n");    //；立刻调用
    if(ret == -1) {
        perror("setitimer");
        exit(0);
    }
    getchar();  //捕获键盘输入，在这里的作用是阻塞main，等待定时器触发
    return 0;
}
