#include <func.h>

int main()
{
    printf("getpid = %d\n getppid = %d\n getuid = %d\n geteuid = %d\n getgid = %d\n getegid = %d\n"
           ,getpid(),getppid(),getuid(),geteuid(),getgid(),getegid());
    return 0;
}

