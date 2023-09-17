/*
    #include <pthread.h>
    int pthread_detach(pthread_t thread);

        - 功能：分离一个线程。被分离的线程在终止的时候，会自动释放资源返回给系统。
        (分离线程和exit一个线程有什么区别)
          1.不能多次分离，会产生不可预料的行为。
          2.不能去连接一个已经分离的线程，会报错。
        - 参数：需要分离的线程的ID
        - 返回值：
            成功：0
            失败：返回错误号

    分离一个线程（Detaching a thread）：在多线程编程中，当一个线程被创建后，可以选择将其分离。
    分离一个线程意味着该线程的资源（如内存和系统资源）将在线程完成执行后自动被释放，而不需要其他线程显式地等待该线程结束。
    分离线程可以使得主线程或其他线程能够继续执行，而不需要等待被分离的线程完成。
    分离线程通常用于不需要与主线程或其他线程进行交互或同步的任务。
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


void * callback(void * arg) {
    printf("chid thread id : %ld\n", pthread_self());
    return NULL;
}

int main() {

    // 创建一个子线程
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error1 : %s\n", errstr);
    }

    // 输出主线程和子线程的id
    printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());

    // 设置子线程分离,子线程分离后，子线程结束时对应的资源就不需要主线程释放
    ret = pthread_detach(tid);
    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error2 : %s\n", errstr);
    }

    // 设置分离后，对分离的子线程进行连接 pthread_join()
    ret = pthread_join(tid, NULL);
    if(ret != 0) {
        char * errstr = strerror(ret);
        printf("error3 : %s\n", errstr);
    }

    pthread_exit(NULL);

    return 0;
}