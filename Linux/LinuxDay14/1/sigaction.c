#include <func.h>


void sigfunc(int signum, siginfo_t *p, void *p1)
{
    printf("%d\n", signum);
    sleep(3);
    printf("%d is coming\n", signum);
}

int main()
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_flags = SA_SIGINFO|SA_RESTART;
    act.sa_sigaction = sigfunc;
    int ret1 = sigaction(SIGINT,&act, NULL);
    int ret2 = sigaction(SIGQUIT,&act, NULL);
    while(1);
    return 0;
}

