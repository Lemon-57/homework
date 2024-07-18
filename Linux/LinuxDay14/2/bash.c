//(b).  实现一个简单的bash程序，2号信号到来时，
//打印一下当前的路径，可以创建子进程执行其他程序，父进程等待
//其他内容自己添加，比如输入exit就退出，ls命令等等。
//while(1){
  //	打印当前工作路径
  //	read stdin
  //	fork exec
  //	wait
 // }
#include <func.h>


void sigfunc(int signum, siginfo_t *p, void *p1)
{
    char *buf1  = getcwd(NULL,0);
    puts(buf1);
    printf("%d is coming\n", signum);
    char buf2[1024] = {0};
    
        while(1)
        {
             read(STDIN_FILENO,buf2,sizeof(buf2));
             if(strncmp(buf2,"exit",4) == 0){
                printf("buf2 is %s",buf2);
                exit(0);
             }
             pid_t pid = fork();
             if(pid == 0){   
                execle("./print","print",NULL,NULL);
                exit(0);
             }
             wait(NULL);
        }
    
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

