#include <func.h>

int main()
{
    printf("Hello world\n");
    sigset_t maskset;
    sigfillset(&maskset);

    sigset_t oldmaskset;
    int ret = sigprocmask(SIG_BLOCK,&maskset,&oldmaskset);
    ERROR_CHECK(ret,-1,"sigprocmask");
    
    sleep(3);
    ret = sigprocmask(SIG_SETMASK,&oldmaskset,NULL);
    ERROR_CHECK(ret,-1,"sigprocmask");
    
    while(1);
    return 0;
}

