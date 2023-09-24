/*
    读写锁的类型 pthread_rwlock_t
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

    案例：8个线程操作同一个全局变量。
    3个线程不定时写这个全局变量，5个线程不定时的读这个全局变量
*/
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int num = 1;    //全局变量
pthread_rwlock_t rwlock;
void* callback_w(void* arg) {    
    //不定时写
    while(1){
        usleep(100);
        pthread_rwlock_wrlock(&rwlock); //上读写锁
        // int a = rand(); //随机生成一个num
        num++;    //写操作
        pthread_rwlock_unlock(&rwlock); //解读写锁
    }

    return NULL;
}
void* callback_r(void* arg) {    
    //不定时读
    while(1){
        usleep(100);
        pthread_rwlock_rdlock(&rwlock); //上读锁
        printf("num = %d\n",num);
        pthread_rwlock_unlock(&rwlock); //解读锁
    }
    return NULL;
}
int main(void) {
    pthread_rwlock_init(&rwlock,NULL);  //初始化读写锁
    pthread_t thread1,thread2,thread3,thread4,thread5,thread6,thread7,thread8;  //声明8个线程ID
    int ret1 = pthread_create(&thread1,NULL,callback_w,NULL);    //创建9个线程
    int ret2 = pthread_create(&thread2,NULL,callback_w,NULL);
    int ret3 = pthread_create(&thread3,NULL,callback_w,NULL);
    int ret4 = pthread_create(&thread4,NULL,callback_r,NULL);
    int ret5 = pthread_create(&thread5,NULL,callback_r,NULL);
    int ret6 = pthread_create(&thread6,NULL,callback_r,NULL);
    int ret7 = pthread_create(&thread7,NULL,callback_r,NULL);
    int ret8 = pthread_create(&thread8,NULL,callback_r,NULL);
    // 回收子线程的资源,阻塞
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);
    //摧毁读写锁
    pthread_rwlock_destroy(&rwlock);
    pthread_exit(NULL); // 退出主线程
    return 0;
}