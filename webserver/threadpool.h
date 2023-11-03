#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <pthread.h>
#include <list>
#include "locker.h"
#include "sem.h"
#include <cstdio>

template <typename T>
class threadpool{
    private:
        //线程的数量
        int m_thread_number;

        //线程池数组，大小为m_thread_number
        pthread_t* m_threads;

        //请求队列中，最大运行等待处理的请求数量
        int m_max_requests;
        
        //请求（工作）队列
        std::list <T*> m_workqueue;

        //互斥锁
        locker m_queuelocker;

        //信号量：用来判断是否有任务需要处理
        sem mqueuesem;

        //是否需要结束线程
        bool m_stop;
    private:
        
        //创建线程时传入的执行函数
        static void* worker(void* arg);

        //线程池启动函数
        void run();
    public:
        //构造函数
        threadpool(int thread_number = 8,int max_requests = 10000);   //线程数量默认8，默认最大请求数量10000

        ~threadpool();

        bool append(T* request);


};
//构造函数
template<typename T>
threadpool<T>::threadpool(int thread_number,int max_requests){
    
    //初始化线程池中的参数
    m_thread_number = thread_number;
    m_max_requests = max_requests;
    m_stop = false;
    m_threads = NULL;

    //对于非法传入的参数->抛出异常
    if((thread_number <=0) || (max_requests <=0)){ 
        throw std::exception();
    }

    //使用 new 运算符来动态分配一个 pthread_t 类型的数组，数组的大小由 thread_number 决定
    m_threads = new pthread_t[thread_number];
    if(!m_threads){
        throw std::exception();
    }
    //创建thread_number个线程，并将他们装入数组
    //逐个获取 m_threads 数组中每个元素的地址，并将这些地址作为参数传递给 pthread_create()，从而将创建的线程标识符存储到数组中的相应位置。以便后续使用
    for(int i = 0; i < thread_number; i++) {
        printf(" thread %d create ok\n",i);
        int ret = pthread_create(&m_threads[i], NULL,worker, this);    //等价于pthread_create(m_threads+i, NULL, , NULL);
        if(ret != 0) {
            delete [] m_threads;    //创建线程失败，释放内存
            throw std::exception(); //抛出异常
        }
        //分离线程，线程完成执行后将自动被释放
        if(pthread_detach(m_threads[i]) !=0 ){//注意这里传递的是线程标识符而不是指向线程标识符的指针
            delete [] m_threads;    //创建线程失败，释放内存
            throw std::exception(); //抛出异常
        }   
    }



}

//析构函数
template<typename T>
threadpool<T>::~threadpool(){
    //释放线程数组的内存、避免内存泄漏
    delete[] m_threads; //  delete[] 对应 new []
    m_stop = true;
}

//向线程池中append
template<typename T>
bool threadpool<T>::append(T* request){
    //添加任务要首先保证线程同步
    //上锁
    bool lock = m_queuelocker.lock();
    if(!lock){
        //上锁失败
        printf("Lock failed\n");
        return false;
    }
    //判断请求队列容量是否足够
    if(m_workqueue.size() > m_thread_number){
        //请求队列的体积超出线程池最大容量->提示用户并抛出异常
        printf("Insufficient capacity\n");
        bool unlock = m_queuelocker.unlock();
        if(!unlock){
            //解锁失败
            printf("Unlock failed\n");
            return false;
        }
        return false;   //添加失败
    }
    m_workqueue.push_back(request); //将请求存入
    bool unlock = m_queuelocker.unlock();
    if(!unlock){
        //解锁失败
        printf("Unlock failed\n");
        return false;
    }
    mqueuesem.post();   //信号量增加(置1)
    return true;
}

template <typename T>
void* threadpool<T>::worker(void* arg){
    threadpool* mypool = (threadpool*)arg;
    mypool->run();
    return NULL;
}

template <typename T>
inline void threadpool<T>::run()
{
    while(!m_stop){
        mqueuesem.wait();
        bool lock = m_queuelocker.lock();
        if(!lock){
            //上锁失败
            printf("Lock failed\n");
            throw std::exception();
        }
        //如果请求队列为空，则该线程解锁
        if(m_workqueue.empty()){
            bool unlock = m_queuelocker.unlock();
            if(!unlock){
                //解锁失败
                printf("Unlock failed\n");
                throw std::exception();
            }
            continue;//当队列为空时(请求被别的任务线程处理了~)跳出if继续循环
        }
        //取出头节点，并将list中的头节点弹出，
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        //解锁
            bool unlock = m_queuelocker.unlock();
            if(!unlock){
                //解锁失败
                printf("Unlock failed\n");
                throw std::exception();
            }
        //请求为空
        if(!request){
            continue;
        }
        //处理请求
        request->process();
    }
}
#endif