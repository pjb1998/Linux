#include <stdio.h>

int main() {
    int a;
    float b;
    char c;
    a = 3;
    b = 4.0;
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of float: %zu bytes\n", sizeof(float));
    printf("Size of char: %zu bytes\n", sizeof(char));
    
    printf("Size of variable a: %zu bytes\n", sizeof(a));
    printf("Size of variable b: %zu bytes\n", sizeof(b));
    printf("Size of variable c: %zu bytes\n", sizeof(c));
    
    return 0;
}
