/*  
    字节序：字节在内存中存储的顺序。
    小端字节序：数据的高位字节存储在内存的高位地址，低位字节存储在内存的低位地址
    大端字节序：数据的低位字节存储在内存的高位地址，高位字节存储在内存的低位地址
*/

// 通过代码检测当前主机的字节序
#include <stdio.h>

int main() {
    /*
    
    */
    union {
        short value;    // 占2字节
        char bytes[sizeof(short)];  // char[2] char占一个字节，定义一个两个字节大的char数组
    } test;
    /*
    计算机硬件有两种储存数据的方式： 大端字节序（big endian）和小端字节序（little endian）。 
    比如十六进制数0x0102，十六进制每个数字占4bit，四个数字16bit，也就是2byte，2个字节存储。 
    0x0102高位字节是0x01，低位字节是0x02。
    */
    test.value = 0x0102;
    if((test.bytes[0] == 1) && (test.bytes[1] == 2)) {
        printf("大端字节序\n");
    } else if((test.bytes[0] == 2) && (test.bytes[1] == 1)) {
        printf("小端字节序\n");
    } else {
        printf("未知\n");
    }
    return 0;
}