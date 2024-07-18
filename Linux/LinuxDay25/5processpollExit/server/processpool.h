 ///
 /// @file    processpool.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-01 20:20:27
 ///
 
#ifndef __WD_PROCESSPOOL_H__
#define __WD_PROCESSPOOL_H__

#include "tcp.h"
#include "transferFd.h"

#include <func.h>

#define MAXCONNS 1000
#define SENDBUFFSIZE 1000
#define BUFFSIZE 1000
#define PROCESS_NOT_EXIT_FLAG 0
#define PROCESS_READY_EXIT_FLAG 1

typedef struct {
	pid_t pid; //子进程的id
	int pipefd;//子进程与父进程进行通信的管道
	int busy;  //子进程的状态, 1 表示忙碌，0表示空闲
} process_pool_t, *pProcess_pool_t;

typedef struct {
	int length;
	char data[BUFFSIZE];
} train_t;
 
void makeChild(pProcess_pool_t pool, int processNum);
void childHandleTask(int pipefd);//
int transferFile(int peerfd, const char * filename);
 
 
#endif
