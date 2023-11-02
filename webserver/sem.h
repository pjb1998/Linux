#ifndef SEM_H_
#define SEM_H_
#include <semaphore.h>
#include <pthread.h>
#include <exception>
class sem {
    public:
        sem(int pshared = 0,unsigned int value = 0){
            //初始化信号量
            if(sem_init(&m_sem,pshared,value) != 0){
                throw std::exception();
            }; 
        }
        ~sem(){
            sem_destroy(&m_sem); //释放信号量
        }
        //对信号量加锁(消除)，调用一次对信号量的值-1，如果值为0，就阻塞
        bool wait(){
            return (sem_wait(&m_sem) == 0);
        }
        //尝试对信号量进行加锁，如果信号量的值为0，线程不会被阻塞，而是立即返回失败码（这里暂时不返回失败码）
        bool tryWait(){
           return (sem_trywait(&m_sem) == 0); 
        }
        /*
            timedWait ：超时时间的等待函数
            1、如果信号量的值大于 0，它会立即减少信号量的值并返回 0，表示成功获取到信号量。
            2、如果信号量的值为 0，则线程会等待超时时间，直到以下情况之一发生：
                成功获取到信号量，返回 0；
                超时时间到达，未能获取到信号量，返回一个非零的错误码。
            3、如果在等待期间发生了错误，返回一个负的错误码。
        */
        bool tryTimeWait(const struct timespec *abs_timeout){
           return (sem_timedwait(&m_sem,abs_timeout) == 0); 
        }
        //对信号量解锁(投掷)，调用一次对信号量的值+1
        bool post(){
            return (sem_post(&m_sem) == 0);
        }
        //用于获取某个信号量中的具体值,利用传入传出参数返回互斥量中的值
        int getValue(){
            int temp;
            sem_getvalue(&m_sem,&temp);
            return temp;
        }
    private:
        sem_t m_sem;
};
#endif