#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <exception>

//创建线程同步_互斥锁类
class locker {
    public:
        //构造函数
        locker(){
            if(pthread_mutex_init(&m_mutex,NULL) != 0){
                throw std::exception(); //互斥锁创建失败，抛出异常对象
            }
        }
        //析构函数
        ~locker(){
            pthread_mutex_destroy(&m_mutex); //在析构函数中销毁互斥锁
        }
        //提供上锁的函数
        bool lock(){      
            return (pthread_mutex_lock(&m_mutex) == 0); //=0说明加锁成功，否则加锁失败
        }
        //提供解锁的函数
        bool unlock(){      
            return (pthread_mutex_unlock(&m_mutex) == 0); //=0说明解锁成功，否则解锁失败
        }
        pthread_mutex_t* getlock(){
            return &m_mutex;
        }
    private:
        //创建互斥锁（私有）
        pthread_mutex_t m_mutex;
};


#endif