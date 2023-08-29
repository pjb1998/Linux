/*
    #include <stdlib.h>
    void exit(int status);

    #include <unistd.h>
    void _exit(int status);

    status参数：是进程退出时的一个状态信息。父进程回收子进程资源的时候可以获取到。
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    printf("hello\n");
    printf("world");
    // exit(0);     //刷新I/O缓冲 
    _exit(0);       //不刷新I/O缓冲
    printf("after exit\r\n");
    return 0;   //这里的return和exit都表示进程退出
}