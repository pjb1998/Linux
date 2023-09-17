/*
    一般情况下,main函数所在的线程我们称之为主线程（main线程），其余创建的线程
    称之为子线程。
    程序中默认只有一个进程，fork()函数调用，2个进程。
    程序中默认只有一个线程，pthread_create()函数调用，2个线程。

    #include <pthread.h>
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
    void *(*start_routine) (void *), void *arg);

        - 功能：创建一个子线程
        - 参数：
            - thread：传出参数，线程创建成功后，子线程的线程ID被写到该变量中。
            - attr : 设置线程的属性，一般使用默认值，NULL
            - start_routine : 函数指针，这个函数是子线程需要处理的逻辑代码
            - arg : 给第三个参数使用，传参
        - 返回值：
            成功：0
            失败：返回错误号。这个错误号和之前errno不太一样。
            获取错误号的信息：  char * strerror(int errnum);
        错误说明:
        panjinbao@pan-Ubuntu:~/Linux/lession29$ cc pthread_create.c -o create
        /tmp/ccw9u8Zu.o：在函数‘main’中：
        pthread_create.c:(.text+0x4f)：对‘pthread_create’未定义的引用
        collect2: error: ld returned 1 exit status

        查 man 3 pthread_create发现----> Compile and link with -pthread.

        call_back未能打印出“hello,world,child thread create”
        原因：
        子线程还没创建完成，主线程已经结束return了
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* call_back(void * a){ //回调函数
    printf("hello,world,child thread create\n");
    printf("arg value : %d \n",*(int *)a);
    return NULL;
}

int main(void) {
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
    sleep(1);
    return 0;
}