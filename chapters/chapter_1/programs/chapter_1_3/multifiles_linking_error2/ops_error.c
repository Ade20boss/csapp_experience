// ops.c

#include <stddef.h>
#include "ops.h"

void slow_add(int* dest, int iterations) {
    for (size_t i = 0; i < iterations; i++) {
        *dest += 1;
    }
}

void fast_add(int* dest, int iterations) {
    int temp = 0;
    for (size_t i = 0; i < iterations; i++) {
        temp += 1;
    }
    *dest = temp;
}
