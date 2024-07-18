#include <func.h>

int main()
{
    int i = 0;
    for(i = 0; i < 2; i++)
    {
        pid_t pid = getpid();
        fork();
        printf("-\n");
        printf("ppid = %d \n pid = %d \n", getppid(),getpid());
    }
    return 0;
}

