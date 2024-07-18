#include "tcp_epoll.h"

int tcpInit(char *ip, char *port, int *pSockFd)
{
    *pSockFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    int reuse = 1;
    int ret;
    ret = setsockopt(*pSockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");
    ret = bind(*pSockFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(*pSockFd, 100);
    ERROR_CHECK(ret, -1, "listen");
    return 0;
}

//epoll相关操作封装
int epollCtor(){
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_createl");
    return epfd;
}

//初始化为只监听读事件epollAddFd
int epollAddFd(int epfd, int fd){
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    
    event.events = EPOLLIN;
    event.data.fd = fd;
    
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl AddFd");
    
    return 0;
}

//初始化监听读事件epollAddTimeFd
int epollAddTimeFd(int epfd, int fd)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    //设置为边缘触发,每当状态变化时发生一个io事件
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET; 
    
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl epollAddTimeFd");
    
    return 0;
}
//删除监听
int epollDel(int epfd, int fd){
    struct epoll_event event;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl Del");
    return 0;
}
