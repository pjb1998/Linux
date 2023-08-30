/*
	#include <sys/types.h>
   #include <sys/wait.h>

   pid_t wait(int *wstatus);

   pid_t waitpid(pid_t pid, int *wstatus, int options);

   int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
   功能：等待任意一个子进程结束，如果任意一个子进程结束了，次函数会回收子进程的资源。
   参数：int *wstatus
   进程退出时的状态信息，传入的是一个int类型的地址，传出参数。
   返回值：
   - 成功：返回被回收的子进程的id
   - 失败：-1 (所有的子进程都结束，没有子进程可以回收，调用函数失败无法回收子进程)

   调用wait函数的进程会被挂起（阻塞），直到它的一个子进程退出或者收到一个不能被忽略的信号时才被唤醒（相当于继续往下执行）
   如果没有子进程了，函数立刻返回，返回-1；如果子进程都已经结束了，也会立即返回，返回-1.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
   pid_t pid;
   for(int i = 0;i<5;i++){
      pid = fork();
      if(pid == 0){
         break;
      }
   }
   if (pid > 0){
      //这是父进程
      while(1){
         printf("parent,pid = %d\n",getpid());

         int ret = wait(NULL);
         printf("ret = %d\n",ret);

         sleep(1);
      }
   }else if(pid == 0){
      //这是子进程
      printf("child,pid = %d\n",getpid());
      while(1);
   }
   return 0;
}

