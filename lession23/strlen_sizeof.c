#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char *c = "panjinbao";
    char *c2 = "pa";
    int a = 10;
    printf("stelen = %ld\n",strlen(c));     //9
    printf("sizeof = %ld\n",sizeof(c));     //8
    return 0;
}
/*

    strlen() 函数返回字符串的长度，即字符的个数（不包括结尾的空字符 \0），而 sizeof() 运算符返回变量或类型的字节数。
    解释如下：
    strlen(c)：这里的 c 是一个指向字符串常量 "panjinbao" 的指针。strlen(c) 返回的是字符串中字符的个数（不包括结尾的空字符 \0）。在这个例子中，字符串 "panjinbao" 由 9 个字符组成，所以 strlen(c) 返回值为 9。
    sizeof(c)：这里的 c 是一个指向字符串常量 "panjinbao" 的指针。sizeof(c) 返回的是指针变量 c 所占的字节数。在大多数系统上，指针变量的大小通常是 8 个字节（64位系统）。因此，sizeof(c) 返回值为 8。
    需要注意的是，sizeof(c) 返回的是指针变量 c 的大小，而不是字符串的大小。指针变量存储的是字符串的内存地址，而不是字符串本身。
    另外，需要指出的是，sizeof() 在这里计算的是指针变量的大小，而不是字符串的实际长度。如果要获取字符串的长度，应该使用 strlen() 函数。
    总结来说，strlen(c) 返回字符串 "panjinbao" 的长度（9），而 sizeof(c) 返回指针变量 c 的大小（8）。
*/