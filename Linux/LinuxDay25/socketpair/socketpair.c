#include<func.h>
#include"transferFd.h"

int main(int argc, char *argv[])
{
    int fds[2];
    //创建本地套接字SOCK_STREAM(tcp协议)fds(存储管道两端的文件描述符)
    socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
    printf("fds[0]: %d, fds[1]: %d\n", fds[0], fds[1]);

    pid_t pid = fork();
    if(pid > 0){
        close(fds[0]);//父进程关闭读端
        //fd模拟的是peerfd
        printf("file parent pid %d\n",getpid() );
        char *buf = "hello world";
        write(fds[1],"hello",strlen(buf));
        wait(NULL);
    }else{
        close(fds[1]);//关闭写段
        printf("file child pid %d\n", getpid());
        char buf[20] = {0};
        read(fds[0], buf, sizeof(buf));
        printf("from parent %s\n",buf);
    }
}

