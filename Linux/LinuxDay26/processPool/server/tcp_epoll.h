#include<func.h>

int tcpInit(char *ip, char *port, int *pSockFd);

int epollCtor();

int epollAdd(int fd, int epfd);

int epollDel(int fd, int epfd);
