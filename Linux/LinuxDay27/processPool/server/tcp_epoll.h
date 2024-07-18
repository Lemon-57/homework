#include<func.h>

int tcpInit(char *ip, char *port, int *pSockFd);

int epollCtor();
//初始化为只监听读事件
int epollAddFd(int epfd, int fd);
//初始化监听读写事件
int epollAddTimeFd(int epfd, int fd);
//删除监听
int epollDel(int epfd, int fd);
