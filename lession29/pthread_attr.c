/*
    操作：man pthread_attr_+tab+tab
    panjinbao@pan-Ubuntu:~/Linux/lession29$ man pthread_attr_
    pthread_attr_destroy          pthread_attr_init
    pthread_attr_getaffinity_np   pthread_attr_setaffinity_np
    pthread_attr_getdetachstate   pthread_attr_setdetachstate
    pthread_attr_getguardsize     pthread_attr_setguardsize
    pthread_attr_getinheritsched  pthread_attr_setinheritsched
    pthread_attr_getschedparam    pthread_attr_setschedparam
    pthread_attr_getschedpolicy   pthread_attr_setschedpolicy
    pthread_attr_getscope         pthread_attr_setscope
    pthread_attr_getstack         pthread_attr_setstack
    pthread_attr_getstackaddr     pthread_attr_setstackaddr
    pthread_attr_getstacksize     pthread_attr_setstacksize

    pthread_attr_t attr;
        - 创建一个线程属性变量
    int pthread_attr_init(pthread_attr_t *attr);
        - 初始化线程属性变量

    int pthread_attr_destroy(pthread_attr_t *attr);
        - 释放线程属性的资源

    int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
        - 获取线程分离的状态属性

    int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
        - 设置线程分离的状态属性
*/
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void * callback(void * arg) {
    printf("chid thread id : %ld\n", pthread_self());
    return NULL;
}

int main() {
    //创建一个线程属性变量
    pthread_attr_t attr;
    //初始化线程属性变量
    pthread_attr_init(&attr);
    //设置分离属性属性
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    // 创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error1 : %s\n", errstr);
    }
    //获取线程栈的size
    size_t * stackSize = 0;
    pthread_attr_getstacksize(&attr,&stackSize);    //8388608
    printf("stackSize = %ld\n",stackSize);
    // 输出主线程和子线程的id
    printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    return 0;
}