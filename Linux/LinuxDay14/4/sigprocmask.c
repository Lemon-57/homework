#include <func.h>
// 通过sigprocmask阻塞2号信号，睡眠5秒后，解除阻塞，2号信号得到执行；
// 在睡眠后，解除阻塞之前，通过sigpending检测是否有信号挂起

int main()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    int ret = sigprocmask(SIG_BLOCK, &mask, NULL);
    ERROR_CHECK(ret, -1, "sigprocmask");
    printf("block success!\n");
    sleep(5);
    sigset_t pend;
    sigemptyset(&pend);
    sigpending(&mask);
    if (sigismember(&mask, SIGINT))
    {
        printf("SIGINT is pending!\n");
    }
    else
    {
        printf("SIGINT is not pending!\n");
    }
    ret = sigprocmask(SIG_UNBLOCK, &mask, NULL);
    ERROR_CHECK(ret, -1, "sigprocmask");
    while (1)
        ;
    return 0;
}
