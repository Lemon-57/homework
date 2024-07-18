#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc, 3);
    int sockFd = socket(AF_INET,SOCK_STREAM,0);//(ipv4,TCP,传输协议编号)
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));//主机字节序转网络字节序
    addr.sin_addr.s_addr = inet_addr(argv[1]);//网络地址字符串转转网络字节序
    int ret = connect(sockFd,(struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    char buf[1024] = {0};
    read(STDIN_FILENO, buf, sizeof(buf));
    send(sockFd,buf,strlen(buf)-1,0);
    memset(&addr,0,sizeof(addr));
    

    recv(sockFd,buf,sizeof(buf),0);
    puts(buf);

    close(sockFd);
    return 0;
}

