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

int sendn(int fd, const void *buf, int len)
{
    int left = len;
    char * pbuf = (char *)buf;
    int ret;
    while(left > 0){
        ret = send(fd, buf, left, 0);
        if(ret < 0){
            perror("send");
        }else{
            pbuf += ret;
            left -= ret;
        }
    }
    return len - left;
}

void channelInit(channel_t *p)//初始化一个通道
{
    if(p){
        p->sockfd = -1; // -1标志未使用
        p->recvbuffsize = 0;
        p->sendbuffsize = 0;
        memset(p->recvbuff,0,BUFFSIZE);
        memset(p->sendbuff,0,BUFFSIZE);
    }
}

void channelDestroy(channel_t *p)///针对一个通道的销毁
{
    if(p){
        close(p->sockfd);
        channelInit(p);
    }
}

void channelsInit(channel_t *p,int length)//初始化length长度的通道数组
{
    for(int i = 0; i < length; ++i){
        channelInit(p+i);
    }
}

void clearSendBuff(channel_t *p)//清空发送缓冲区
{
    if(p){
        p->sendbuffsize = 0;
        memset(p->sendbuff, 0, BUFFSIZE);
    }
}

void clearRecvBuff(channel_t *p)//清空接收缓冲区
{
    if(p){
        p->recvbuffsize = 0;
        memset(p->recvbuff, 0, BUFFSIZE);
    }
}

void channelAdd(channel_t *p, int length, int fd)//往通道数组中添加通道
{
    for(int i = 0; i < length; ++i){
        if(p[i].sockfd == -1){
            clearRecvBuff(p+i);
            clearSendBuff(p+1);
            break;
        }
    }
}

void channelDel(channel_t *p, int length, int fd)//删除通道数组中的通道
{
    int idx = channelGetIndex(p, length, fd);
    if(idx != -1){
        channelDestroy(p + idx);
    }
}

// 找到返回下标，没找到返回-1
int channelGetIndex(channel_t *p, int length, int fd)//在通道数组中查找
{
    if(p){
        for(int i = 0; i < length; ++i){
            if(p[i].sockfd = fd){
                return i;
            }
        }
        return -1;
    }
}

//epoll相关操作封装
//初始化监听读写事件
void epollAddReadWriteEvent(int epfd, int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN | EPOLLOUT; //关注读写
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt) < 0){
        perror("epoll_ctl");
    }
}

//初始化为只监听读事件
void epollAddReadEvent(int epfd, int fd)
{
    struct epoll_event evt;
    evt.data.fd =fd;
    evt.events = EPOLLIN;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt) < 0){
        perror("epoll_ctl");
    }
}
//修改为监听读写事件
void epollClearWriteREvent(int epfd, int fd)
{
    struct epoll_event evt;
    evt.data.fd =fd;
    evt.events = EPOLLIN | EPOLLOUT;
    if(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &evt) < 0){
        perror("epoll_ctl");
    }
}
//修改为监听读写事件
void epollSetWriteEvent(int epfd, int fd)
{
    struct epoll_event evt;
    evt.data.fd =fd;
    evt.events = EPOLLIN;
    if(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &evt) < 0){
        perror("epoll_ctl");
    }
}
//删除监听
void epollDelEvent(int epfd, int fd)
{
    struct epoll_event evt;
    evt.data.fd =fd;
    if(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &evt) < 0){
        perror("epoll_ctl");
    }
}
