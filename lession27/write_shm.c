#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    //1.创建一个共享内存
    int shmid = shmget(100,4096,IPC_CREAT|0664);
    if(shmid == -1) {
        perror("shmget");
        exit(0);
    }
    printf("share memory id = %d\n",shmid);
    //2.和当前进程进行关联
    //调用成功时返回一个指向共享内存第一个字节的指针，
    //如果调用失败返回-1.
    void * ptr = shmat(shmid,NULL,0);

    //3.写数据
    char * str = "hello world";
    memcpy(ptr,str,strlen(str)+1);

    printf("按任意键继续\n");
    getchar();

    //4.解除关联
    shmdt(ptr);

    //5.删除共享内存
    shmctl(shmid,IPC_RMID,NULL);

    return 0;
}