// main.c
#include <stdio.h>
#include <time.h>
#include "ops.h" // Tells main.c what functions are available


int main() {
    int num = 0;
    slow_add(&num, 1000000000);
    fast_add(&num, 1000000000);
    printf("Done!\n");
    return 0;
}