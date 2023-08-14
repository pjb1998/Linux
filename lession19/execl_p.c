#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
/*  
    #include <unistd.h>
    int execl(const char *path, const char *arg, ...);
        - 参数：
            - path:需要指定的执行的文件的路径或者名称
                a.out /home/nowcoder/a.out 推荐使用绝对路径
                ./a.out hello world

            - arg:是执行可执行文件所需要的参数列表
                第一个参数一般没有什么作用，为了方便，一般写的是执行的程序的名称
                从第二个参数开始往后，就是程序执行所需要的的参数列表。
                参数最后需要以NULL结束（哨兵）

        - 返回值：
            只有当调用失败，才会有返回值，返回-1，并且设置errno
            如果调用成功，没有返回值。

*/
int main(){
    pid_t pid = fork();
    if(pid > 0 )
    {
        //这是父进程
        for(int i = 0;i<10;i++){
            printf("I am parent\r\n");
            sleep(1);
        }

    }else if(pid == 0){
        //这是子进程
        printf("I am son\r\n");
        execl("hello","hello",NULL);
    }
    return 0;
}