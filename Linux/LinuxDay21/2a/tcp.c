#include"tcp.h"

void setNonblock(int fd)//针对accept返回的文件描述符设置非阻塞
{
    int flags = fcntl(fd,F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

struct sockaddr_in inetaddress(const char *ip,uint16_t port){//初始化结构体成员
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;//使用ipv4地址
    serverAddr.sin_addr.s_addr = inet_addr(ip);//点分十进制字符串转换网络主机号
    serverAddr.sin_port = htons(port);
    return serverAddr;
}

int socketInit(){//初始化socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(listenfd,-1,"socket");
    return listenfd;
}

int tcpServerInit(const char * ip, uint16_t  port){//建立TCP连接
    //创建TCP套接字
    int listenfd = socketInit();
    //创建结构体并设置属性
    struct sockaddr_in serverAddr = inetaddress(ip,port);
    
    //设置套接字属性，让网络地址可以重用
    int resuse =1;
    int ret = setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&resuse,sizeof(resuse));
    ERROR_CHECK(ret, -1, "setsockopt");
    //绑定IP地址
    ret = bind(listenfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind");
    
    ret = listen(listenfd,100);
    ERROR_CHECK(ret, -1, "listen");
    printf("connect has complete");
    return listenfd;
}
