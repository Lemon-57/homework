#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd, -1," open");
    write(fd,"hello",5);
    printf("Hello world\n");
    sleep(10);
    return 0;
}

