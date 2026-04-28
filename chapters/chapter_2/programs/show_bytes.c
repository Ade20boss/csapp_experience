#include <stdio.h>
#include <stdint.h>



void show_bytes(uint8_t *start, size_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        printf("%.2x  ", start[i]);
    }
    printf("\n");
}

void show_int(int *x)
{
    show_bytes((uint8_t *)x, sizeof(int));
}

void show_float(float *x)
{
    show_bytes((uint8_t *)x, sizeof(float));
}

void show_pointer(void *x)
{
    show_bytes((uint8_t *)x, sizeof(void*));
}


int main()
{
    int val = 87777;
    float f = 45.788;
    void *ptr = &f;


    show_int(&val);
    show_float(&f);
    show_pointer(ptr);

    return 0;
    
}