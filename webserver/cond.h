#ifndef COND_H_
#define COND_H_
#include <pthread.h>
#include <exception.h>

//创建线程同步_条件变量
class cond {
    public:
        //构造函数
        cond(){
            if(pthread_cond_init(&m_cond, NULL) != 0){
                throw std::exception();//条件变量创建失败，抛出异常对象
            }
        }
        //析构函数
        ~cond(){
            pthread_cond_destroy(&m_cond);//析构函数中释放条件变量
        }
        //唤醒阻塞在该条件变量上的所有线程中的某一个线程、而其他线程仍然会继续等待
        bool noticeOneorMore(){
            return (pthread_cond_signal(&m_cond) == 0); //函数调用返回0表示调用成功
        }
        //唤醒阻塞在该条件变量上的所有线程
        bool noticeAll(){
            return (pthread_cond_broadcast(&m_cond) == 0);
        }
        //调用该函数会先释放互斥锁，如果条件变量未满足：将当前线程阻塞在条件变量上，
        //如果条件变量满足：当前线程继续运行，再获取互斥锁（再加锁），进入下一步执行
        bool waitOnConditionVariable(pthread_mutex_t* m_mtex){
            return (pthread_cond_wait(&m_cond, m_mtex) == 0);
        }
        //带有超时时间的等待
        bool timeWaitOnConditionVariable(pthread_mutex_t* m_mtex,const struct timespec* abstime){
            return (pthread_cond_timedwait(&m_cond, m_mtex,abstime) == 0);
        }
    private:
        // 创建条件变量
        pthread_cond_t m_cond;

};
#endif