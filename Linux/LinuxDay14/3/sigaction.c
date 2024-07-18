#include <func.h>


//. 使用sigaction注册2号信号的处理函数，
//在处理2号信号时阻塞3号信号，不会被3号信号打断，
//同时在2号信号的信号处理函数中，判断一下有没有3号信号处于未决状态。
void sigfunc(int signum, siginfo_t *p, void *p1)
{
    printf("before %d is coming \n", signum);
    sleep(3);
    printf("after %d is coming\n", signum);
}

int main()
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sigfunc;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);
    int ret1 = sigaction(SIGINT,&act, NULL);
    int ret2 = sigaction(SIGQUIT,&act, NULL);
    while(1);
    return 0;
}

