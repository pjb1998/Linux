/*
    https://www.cnblogs.com/yychuyu/p/13741830.html
    使用多线程实现买票的案例。
    有3个窗口，一共是100张票。
*/
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//创建互斥锁
pthread_mutex_t ticketslock = PTHREAD_MUTEX_INITIALIZER;
// 全局变量，所有的线程都共享这一份资源。
int tickets = 100;
void* callback(void* a) {
    // printf("hello,world,child thread1 create\n");
    // pthread_mutex_lock(&ticketslock);   //加锁
    // int needTickets = 10;
    // tickets = 100-needTickets;
    // pthread_mutex_unlock(&ticketslock); //解锁

    // 卖票
    pthread_t self = pthread_self();
    unsigned long int Numlist[3];
    Numlist[0] = 1;
    Numlist[1] = 2;
    Numlist[2] = 3;
    while(tickets > 0) {
        usleep(6000);
        //没写出来，如何将三个id映射到1，2，3上
        printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
        tickets--;
    } 
    return NULL;
}
int main(void) {
    //初始化互斥锁ticketslock
    pthread_mutex_init(&ticketslock,NULL);
    //声明三个线程ID
    pthread_t thread1,thread2,thread3;  //声明线程ID。
    //创建三个线程
    int ret1 = pthread_create(&thread1,NULL,callback,NULL);
    int ret2 = pthread_create(&thread2,NULL,callback,NULL);
    int ret3 = pthread_create(&thread3,NULL,callback,NULL);
    if(ret1 != 0) {
        char * errstr = strerror(ret1);
        printf("error : %s\n",errstr);
    }
    if(ret2 != 0) {
        char * errstr = strerror(ret2);
        printf("error : %s\n",errstr);
    }
    if(ret3 != 0) {
        char * errstr = strerror(ret3);
        printf("error : %s\n",errstr);
    }
    // for(int i = 0;i < 20;i++){
    //     printf("tickets number = %d\n",tickets);
    //     sleep(1);
    // }
    // 回收子线程的资源,阻塞
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    //设置线程分离
    
    pthread_mutex_destroy(&ticketslock);
    pthread_exit(NULL); // 退出主线程
    return 0;
}


