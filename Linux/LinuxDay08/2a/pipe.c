//将标准输出重定向到log1文件，标准错误输出重定向到log2文件。

#include <func.h>


int main(int argc, char *argv[])
{

    ARGS_CHECK(argc,3);
    close(STDOUT_FILENO);
    int fd1 = open(argv[1],O_RDWR);
    ERROR_CHECK(fd1,-1,"open fd1");
    write(fd1,"hello",5);
    printf("fd1=%d\n",fd1);
    close(STDERR_FILENO);
    int fd2 = open(argv[2],O_RDWR);
    ERROR_CHECK(fd2,-1,"open fd2");
    write(fd2,"world",5);   
    printf("fd2=%d\n",fd2);

//    printf("Hello\n");
    

    return 0;
}

