#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int i1 = atoi(argv[1]);
    int i2 = atoi(argv[2]);
    srand((unsigned)time(NULL));
    int a = rand()%(i2 - i1+1)+i1;
    printf("%d\n",a);
    return 0;
}

