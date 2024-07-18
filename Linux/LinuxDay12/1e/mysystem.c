#include <func.h>

int mysystem(char *cmd);

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    mysystem(argv[1]);
    printf("Hello world\n");
    return 0;
}

int mysystem(char *cmd){
    pid_t pid = fork();
    ERROR_CHECK(pid, -1,"fork");
    if(pid == 0)
    {
        execlp("bash","bash","-c",cmd,NULL);
        _exit(127);
    }else{
        waitpid(pid,0,0);
    }
}
