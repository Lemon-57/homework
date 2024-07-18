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

    listen(*pSockFd, 100);
    return 0;
}

int epollCtor(){
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_createl");
    return epfd;
}

int epollAdd(int fd, int epfd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(epfd, -1, "epoll_ctl Add");
    return 0;
}

int epollDel(int fd, int epfd){
    struct epoll_event event;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl Del");
    return 0;
}
