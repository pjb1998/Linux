#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(void){

    //创建管道fifo1
    char *fifoName1 = "fifo1";
    //判断管道是否已经存在
    int retAccess = access(fifoName1,F_OK);
    if(retAccess == -1){    //如果管道不存在则创建管道
        int ret = mkfifo(fifoName1,0664);    //0664八进制
        //判断有名管道是否创建成功
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
        printf("Pipe1 File creat success\n");
    } else { //如果管道存在则打印File exit
        printf("File1 exist\n");
        exit(0);
    }

    //创建fifo2
    char *fifoName2 = "fifo2";
    //判断管道是否存在
    int retAccess2 = access(fifoName2,F_OK);
    if(retAccess2 == -1){
        int ret = mkfifo(fifoName2,0664);
        //判断有名管道是否创建成功
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
        printf("Pipe2 File creat success\n");
    } else { //如果管道存在则打印File exit
        printf("File2 exist\n");
        exit(0);
    }
    //管道创建成功，开始写入数据，以只写的方式打开管道1
    int file1Descriptor = open(fifoName1,O_WRONLY);
    if(file1Descriptor == -1){
        perror("open");
        exit(0);
    }
    printf("打开管道fifo1成功，等待写入...\n");
    //管道创建成功，开始读取数据，以只读的方式打开管道2
    int file2Descriptor = open(fifoName2,O_RDONLY);
    if(file2Descriptor == -1){
        perror("open");
        exit(0);
    }
    printf("打开管道fifo2成功，等待读取...\n");
    char writebuf[1024] = {0};
    char readbuf[1024] = {0};
    //循环的写读数据
    while(1) {
        //清空数组
        memset(writebuf,0,1024);
        memset(readbuf,0,1024);
        //写入数据
        fgets(writebuf,1024,stdin);
        int writeret = write(file1Descriptor, writebuf, strlen(writebuf));
        if(writeret == -1) {
            perror("write");
            break;
        }
        //读取数据数据
        int count = read(file2Descriptor,readbuf,sizeof(readbuf));
        if(count == 0){
            printf("write close\n");
            break;
        } else if(count < 0) {
            perror("read");
        }
        printf("rev buf %s\n",readbuf);
    }
    //关闭管道
    close(file1Descriptor);
    close(file2Descriptor);
    return 0;
}