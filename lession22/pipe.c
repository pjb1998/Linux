/*
    #include <unistd.h>

    int pipe(int pipefd[2]);

    #define _GNU_SOURCE             //See feature_test_macros(7) 
    #include <fcntl.h>              //Obtain O_* constant definitions 
    #include <unistd.h>

    int pipe2(int pipefd[2], int flags);

    功能：创建一个匿名管道，用来进程间通信。
        参数：int pipefd[2] 这个数组是一个传出参数。
        pipefd[0] 对应的是管道的读端
        pipefd[1] 对应的是管道的写端
    返回值：
        成功 0
        失败 -1

    管道默认是阻塞的：如果管道中没有数据，read阻塞，如果管道满了，write阻塞

    注意：匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
int main() {
    int pipefd[2];  //数组
    int ret = pipe(pipefd);   
    if(ret == -1){
        perror("pipe");
        exit(0);
    }
    pid_t pid = fork();     //如果创建pipe放在fork之后，则两个进程对应的不是同一个管道

    // 父进程
    if(pid > 0) {
        //关闭写端;
        close(pipefd[1]);
        // sleep(1);
        // 如果大于0，返回的是创建的子进程的进程号，当前是父进程
        printf("i am parent process, pid : %d, ppid : %d\n", getpid(), getppid());
        //父进程读管道
        char buf[1024] = {0};
        while(1){
            int len = read(pipefd[0], buf, sizeof(buf));    //read默认阻塞的，子进程没写入，父进程就一直读
            printf("parent recv : %s, pid : %d\n", buf, getpid());
        }
    } else if(pid == 0) { //子进程
        //关闭读端
        close(pipefd[0]);
        // 当前是子进程
        printf("i am child process, pid : %d, ppid : %d\n", getpid(),getppid());

        //创建buf流
        // char buf[1024] = {0};
        while(1){
            //子进程写管道
            char * str = "Hello,World";
            write(pipefd[1],str,strlen(str));
            sleep(1);
        }
    }

    // for循环
    // for(int i = 0; i < 3; i++) {
    //     printf("i : %d , pid : %d\n", i , getpid());
    //     sleep(1);
    // }
    
    return 0;
}