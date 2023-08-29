/*
    #include <sys/types.h>
    #include <unistd.h>

    pid_t fork(void);
        函数的作用：用于创建子进程。
        返回值：
            fork()的返回值会返回两次。一次是在父进程中，一次是在子进程中。
            在父进程中返回创建的子进程的ID,
            在子进程中返回0
            如何区分父进程和子进程：通过fork的返回值。
            在父进程中返回-1，表示创建子进程失败，并且设置errno

        父子进程之间的关系：
        区别：
            1.fork()函数的返回值不同
                父进程中: >0 返回的子进程的ID
                子进程中: =0
            2.pcb中的一些数据
                当前的进程的id pid
                当前的进程的父进程的id ppid
                信号集

        共同点：
            某些状态下：子进程刚被创建出来，还没有执行任何的写数据的操作
                - 用户区的数据
                - 文件描述符表
        
        父子进程对变量是不是共享的？
            - 刚开始的时候，是一样的，共享的。如果修改了数据，不共享了。
            - 读时共享（子进程被创建，两个进程没有做任何的写的操作），写时拷贝。
        
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main() {

    int num = 10;
    /*
    具体来说，在fork()函数被调用后，以下情况发生：
    1.在父进程中，fork()函数返回子进程的进程ID（PID），该值大于0。
    这样父进程就可以通过判断返回值来确定自己是父进程。
    2.在子进程中，fork()函数返回0。通过判断返回值为0，子进程可以确定自己是子进程。
    3.如果fork()函数调用失败，返回值为-1，表示创建子进程失败，这通常发生在系统资源不足的情况下。
    */
    // 创建子进程
    pid_t pid = fork(); //根据父进程和子进程中fork函数的返回值pid不同来执行不同的上下文


    // 判断是父进程还是子进程
    if(pid > 0) {
        // printf("pid : %d\n", pid);
        // 如果大于0，fork()返回的是创建的子进程的进程号，当前是父进程
        // 父进程执行中->切换到后台
        // 父进程执行结束->切换到前台panjinbao@pan-Ubuntu:~/Linux/lession20$,但此时子进程还在执行所以产生
        // panjinbao@pan-Ubuntu:~/Linux/lession20$ I am child process, pid : 3730, ppid : 1
        printf("I am parent process, pid : %d, ppid : %d\n", getpid(), getppid());
    } else if(pid == 0) {      
        sleep(1);   //等父进程执行完
        // 当前是子进程
        printf("I am child process, pid : %d, ppid : %d\n", getpid(),getppid());

    }

    // for循环
    for(int i = 0; i < 3; i++) {
        printf("I : %d , pid : %d\n", i , getpid());
    }
    return 0;
}

/*
    读时共享，写时拷贝
    实际上，更准确来说，Linux 的 fork() 使用是通过写时拷贝 (copy- on-write) 实现。
    写时拷贝是一种可以推迟甚至避免拷贝数据的技术。
    内核此时并不复制整个进程的地址空间，而是让父子进程共享同一个地址空间。
    只用在需要写入的时候才会复制地址空间，从而使各个进程拥有各自的地址空间。
    也就是说，资源的复制是在需要写入的时候才会进行，在此之前，只有以只读方式共享。
    注意：fork之后父子进程共享文件，
    fork产生的子进程与父进程相同的文件文件描述符指向相同的文件表，
    引用计数增加，共享文件偏移指针。
*/