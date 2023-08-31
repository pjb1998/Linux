//向管道中写数据
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
/*
    有名管道的注意事项：
        1.一个为只读而打开一个管道的进程会阻塞，直到另外一个进程为只写打开管道
        2.一个为只写而打开一个管道的进程会阻塞，直到另外一个进程为只读打开管道

    读管道：
        管道中有数据，read返回实际读到的字节数
        管道中无数据：
            管道写端被全部关闭，read返回0，（相当于读到文件末尾）
            写端没有全部被关闭，read阻塞等待
    
    写管道：
        管道读端被全部关闭，进行异常终止（收到一个SIGPIPE信号）
        管道读端没有全部关闭：
            管道已经满了，write会阻塞
            管道没有满，write将数据写入，并返回实际写入的字节数。
*/
int main(void){
    //创建管道
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

    //打开管道
    int fileDescriptor = open(fifoname,O_WRONLY);
    if(fileDescriptor == -1){
        perror("open");
        exit(0);
    }

    //写入数据
    for(int i = 0; i < 100; i++) {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i); //hello,1 hello,2...
        printf("write data : %s\n", buf);
        write(fileDescriptor, buf, strlen(buf));
        sleep(1);
    }

    close(fileDescriptor);
    return 0;
}