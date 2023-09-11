#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


int main(void) {
    //1.获取一个共享内存
    int ret = shmget(100,0,IPC_CREAT);
    if(ret == -1) {
        perror("shmget");
        exit(0);
    }
    printf("share memory id = %d\n",ret);

    //2.和当前进程关联
    void * ptr = shmat(ret,NULL,0);
    
    // 3.读数据
    printf("%s\n", (char *)ptr);

    printf("按任意键继续\n");
    getchar();
    // 4.解除关联
    shmdt(ptr);

    // 5.删除共享内存
    shmctl(ret, IPC_RMID, NULL);
    return 0;
}
