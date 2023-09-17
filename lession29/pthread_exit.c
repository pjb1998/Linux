/*
    #include <pthread.h>
    void pthread_exit(void *retval);
        功能：终止一个线程，在哪个线程中调用，就表示终止哪个线程
        参数：
            retval:需要传递一个指针，作为一个返回值，可以在pthread_join()中获取到。

    pthread_t pthread_self(void);
        功能：获取当前的线程的线程ID

    int pthread_equal(pthread_t t1, pthread_t t2);
        功能：比较两个线程ID是否相等
        不同的操作系统，pthread_t类型的实现不一样，有的是无符号的长整型，有的
        是使用结构体去实现的。
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* call_back(void * a){ //回调函数
    pthread_t tid = pthread_self();
    printf("create tid = %ld\n",tid);
    int retval = 1;
    // pthread_exit((void*)&retval);
    printf("hello,world,child thread create\n");
    printf("arg value : %d \n",*(int *)a);
    return NULL;
}

int main(void) {
    printf("main begin\n");
    pthread_t tid = pthread_self();
    printf("main tid = %ld\n",tid);
    int retval = 1;
    int id = 1;
    //创建一个新的线程并打印hello,world
    pthread_t thread;   //typedef unsigned long int pthread_t;
    int ret = pthread_create(&thread,NULL,call_back,(void *)&id);
    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error : %s\n",errstr);
    }

    for(int i = 0;i < 10;i++) {
        printf("%d ",i);
        if(i==9){
            printf("\n");
        }
    }
    // sleep(1);
    //将sleep->pthread_exit(NULL);
    //让主线程退出，当主线程退出时，不会影响其他正常运行的线程
    pthread_exit(NULL);
    printf("main end\n");
    return 0;
}