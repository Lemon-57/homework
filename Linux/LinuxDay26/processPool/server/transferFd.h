#ifndef _TRANSFERFD_H_
#define _TRANSFERFD_H_

int sendFd(int pipeFd, int fdToSend, int exitFlag);

int recvFd(int pipeFd, int *pFd, int *exitFlag);

#endif
