//向管道中读数据
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void){
    char *fifoname = "fifo1";
    //打开管道
    int fileDescriptor = open(fifoname,O_RDONLY);
    if(fileDescriptor == -1){
        perror("open");
        exit(0);
    }
    
    //读出数据
    while(1){
        char buf[1024] = {0};
        int count = read(fileDescriptor,buf,sizeof(buf));
        if(count == 0){
            printf("write close\n");
            break;
        }
        printf("rev buf : %s\n",buf);
    }
    close(fileDescriptor);
    return 0;
}