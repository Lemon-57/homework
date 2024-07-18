#include <func.h>

int main()
{
    int b = 0x12345678;
    char * p = &b;
    printf("%x\n",*p);
    return 0;
}

