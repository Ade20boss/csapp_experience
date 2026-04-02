# 0 "ops.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "ops.c"


# 1 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 1 3 4
# 160 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 3 4

# 160 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 229 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 344 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 3 4
typedef int wchar_t;
# 440 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 451 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 3 4
} max_align_t;
# 465 "/usr/lib/gcc/x86_64-redhat-linux/15/include/stddef.h" 3 4
  typedef __typeof__(nullptr) nullptr_t;
# 4 "ops.c" 2
# 1 "ops.h" 1





# 5 "ops.h"
void slow_add(int* dest, int iterations);
void fast_add(int* dest, int iterations);
# 5 "ops.c" 2

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
