#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

/*
    #include <sys/time.h>
    int setitimer(int which, const struct itimerval *new_value,
                        struct itimerval *old_value);
    
        - 功能：设置定时器（闹钟）。可以替代alarm函数。精度微妙us，可以实现周期性定时
        - 参数：
            - which : 定时器以什么时间计时
                ITIMER_REAL: 真实时间，时间到达，发送 SIGALRM   常用
                ITIMER_VIRTUAL: 用户时间，时间到达，发送 SIGVTALRM
                ITIMER_PROF: 以该进程在用户态和内核态下所消耗的时间来计算，时间到达，发送 SIGPROF

            - new_value: 设置定时器的属性
            
                struct itimerval {      // 定时器的结构体
                    struct timeval it_interval;  // 每个阶段的时间，间隔时间（延迟时间到了也会发SIGALRM信号）
                    struct timeval it_value;     // 延迟多长时间执行定时器（执行定时器时会发送SIGALRM信号）
                };

                struct timeval {        // 时间组成成分的结构体
                    time_t      tv_sec;     //  秒数    
                    suseconds_t tv_usec;    //  微秒    
                };

            过10秒后，每个2秒定时一次
           
            - old_value ：记录上一次的定时的时间参数，一般不使用，指定NULL
        
        - 返回值：
                成功 0
                失败 -1 并设置错误号
*/

//需求：过3秒后，每隔两秒定时一次
int main(void) {
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
    getchar();  //键盘录入
    return 0;
}