#include"function.h"

void sigFunc(int sigNum){
    printf("sig %d is comming\n", sigNum);
    //char ch = 1;
    //通过单独的管道通知父进程退出
    //write(eixiPipeFd[1], &ch, 1);
    exit(0);
}

int socketInit(int sockFd, char *ip, char * port){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);//网络地址字符串转转网络字节序
    addr.sin_port = htons(atoi(port));//主机字节序转网络字节序

    int ret = connect(sockFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    ERROR_CHECK(ret, -1, "connect");
    return 0;
}



