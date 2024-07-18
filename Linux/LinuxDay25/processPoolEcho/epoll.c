#include"epoll.h"

//epoll相关操作封装
//创建epoll实例
int epollCreate(){
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1,"epoll_create");
    return epfd;
}

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

//初始化为监听读事件
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

//修改为监听写事件
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
