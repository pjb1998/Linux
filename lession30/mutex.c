/*
    互斥量的类型 pthread_mutex_t
    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
        - 初始化互斥量
        - 参数 ：
            - mutex ： 需要初始化的互斥量变量
            - attr ： 互斥量相关的属性，NULL
        - restrict : C语言的修饰符，被修饰的指针，不能由另外的一个指针进行操作。
            pthread_mutex_t *restrict mutex = xxx;
            pthread_mutex_t * mutex1 = mutex;  

    int pthread_mutex_destroy(pthread_mutex_t *mutex);
        - 释放互斥量的资源

    int pthread_mutex_lock(pthread_mutex_t *mutex);
        - 加锁，阻塞的，如果有一个线程加锁了，那么其他的线程只能阻塞等待

    int pthread_mutex_trylock(pthread_mutex_t *mutex);
        - 尝试加锁，如果加锁失败，不会阻塞，会直接返回。

    int pthread_mutex_unlock(pthread_mutex_t *mutex);
        - 解锁
*/
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//全局区
//创建互斥锁
pthread_mutex_t ticketslock = PTHREAD_MUTEX_INITIALIZER;
// 全局变量，所有的线程都共享这一份资源。
int tickets = 1000;
void* callback(void* arg) {
    // printf("hello,world,child thread1 create\n");
    // 卖票
    while(1) {
        pthread_mutex_lock(&ticketslock);   //加锁
        if(tickets>0) {
            usleep(6000);
            //没写出来，如何将三个id映射到1，2，3上
            printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
            tickets--;
        }else {
            pthread_mutex_unlock(&ticketslock); //解锁
            break;
        }
        pthread_mutex_unlock(&ticketslock); //解锁
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

