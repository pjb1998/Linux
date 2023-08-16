#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    printf("hello\n");
    printf("world");
    //exit(0);
    _exit(0);
    return 0;
}