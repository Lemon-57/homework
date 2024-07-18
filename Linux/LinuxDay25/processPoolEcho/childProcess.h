#ifndef _CHILDPROCESS_H_
#define _CHILDPROCESS_H_

#include<func.h>
#include"transferFd.h"

typedef struct{
    pid_t pid;//子进程的pid
    int status;//子进程的状态 1表示忙碌 0表示空闲
    int pipeFd;//子进程与父进程通信的管道
}processData_t;

#define FREE 0
#define BUSY 1

//服务端子进程
int makeChild(processData_t *pProcessData, int processNum);
int handleEvent(int pipeFd);

#endif

