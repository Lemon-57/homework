#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int fd[2];
    int fdr,fdw;
    int ret = pipe(fd);
    ERROR_CHECK(ret, -1, "pipe");
    pid_t pid = fork();
    ERROR_CHECK(pid, -1, "fork");

    if(pid == 0){
        close(fd[0]);
        char buf1[1024] = {0};
        fdr = open(argv[1],O_RDONLY);
        read(fdr,buf1,1024);
        puts(buf1);
        write(fd[1],buf1,strlen(buf1));
        exit(0);
    }
    else{
        char buf2[1024] = {0};
        close(fd[1]);
        read(fd[0],buf2,1024);
        puts(buf2);
        fdw = open(argv[2],O_RDWR);
        write(fdw,buf2,strlen(buf2));
        wait(NULL);
        exit(0);
    }

    return 0;
}

