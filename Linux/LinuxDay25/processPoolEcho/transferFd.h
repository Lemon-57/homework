#ifndef _TRANSFER_H_
#define _TRANSFER_H_

int sendFd(int pipefd, int fd);
int recvFd(int pipefd, int *pFd);

#endif
