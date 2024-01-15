#include <stdio.h>
#include "add.hpp"
#include "minus.hpp"

int main(){
    int a=111;
    int b=11;
    int c = minus(a, b);
    int d = add(a, b);
    printf("a-b=%d\n", c);
    printf("a+b=%d\n", d);
    return 0;
}