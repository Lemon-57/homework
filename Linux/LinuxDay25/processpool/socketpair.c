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
        int fd = open("file",O_RDWR);//accept
        printf("file fd %d\n", fd);
        write(fd,"hello",5);
        sendFd(fds[1],fd);//将该fd传给管道的另一端再发送给，让子进程去执行读操作
        wait(NULL);
    }else{
        close(fds[1]);//关闭读段
        int childFd;
        recvFd(fds[0],&childFd);
        printf("file childFd %d\n", childFd);
        char buf[20] = "111";
        read(childFd, buf, strlen(buf));
        printf("recv %s\n",buf);
    }
}

