#ifndef _EPOLL_H__
#define _EPOLL_H_

#include<func.h>

//epoll相关操作封装
int epollCreate();
void epollAddReadWriteEvent(int epfd, int fd);
void epollAddReadEvent(int epfd, int fd);
void epollClearWriteREvent(int epfd, int fd);
void epollSetWriteEvent(int epfd, int fd);
void epollDelEvent(int epfd, int fd);

#endif
