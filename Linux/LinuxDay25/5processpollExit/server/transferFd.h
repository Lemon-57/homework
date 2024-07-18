 ///
 /// @file    transferFd.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-01 11:38:15
 ///
 
#ifndef __WD_TRANSFERFD_H__
#define __WD_TRANSFERFD_H__

int sendFd(int pipefd, int fd, char exitFlag);
int recvFd(int pipefd, char * pexitFlag);
 
 
 
#endif
