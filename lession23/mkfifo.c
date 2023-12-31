/*
    创建fifo文件
    1.通过命令： mkfifo 名字
    2.通过函数：int mkfifo(const char *pathname, mode_t mode);

    #include <sys/types.h>
    #include <sys/stat.h>
    int mkfifo(const char *pathname, mode_t mode);
        参数：
            - pathname: 管道名称的路径
            - mode: 文件的权限 和 open 的 mode 是一样的
                    是一个八进制的数
        返回值：成功返回0，失败返回-1，并设置错误号

*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void){
    char *fifoname = "fifo1";
    //判断管道是否已经存在
    int retAccess = access(fifoname,F_OK);
    if(retAccess == -1){
        int ret = mkfifo(fifoname,0664);
        //判断有名管道是否创建成功
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
        printf("Pipe File creat success\n");
    } else{
        printf("File exit\n");
        exit(0);
    }
    return 0;
}