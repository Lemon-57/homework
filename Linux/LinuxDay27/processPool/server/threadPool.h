#ifndef __WD_THREADPOOL_H__
#define __WD_THREADPOOL_H__

#include "tcp_epoll.h"
#include "taskQueue.h"
#include "command.h"
#include <func.h>

#define MAXCONNS 1000
#define SENDBUFFSIZE 1000
#define BUFFSIZE 1000
#define PROCESS_NOT_EXIT_FLAG 0
#define PROCESS_READY_EXIT_FLAG 1

//线程池信息
typedef struct {
	int threadNumber;//数组长度
	taskQueue_t queue;//任务队列
	pthread_t * threads;//线程id数组(进程内部)
    int threadFlag;//标记进程是否启动 1启动 0未启动
} threadPool_t, *pThreadPool_t;

//子进程信息
typedef struct {
	int length;
	char data[BUFFSIZE];
} train_t;


//线程池
//线程函数
void * threadFunc(void *);
//线程池初始化
void threadpoolInit(pThreadPool_t  threadPool, int threadNum);
//线程池销毁
void threadpoolDestroy(pThreadPool_t threadPool);
//线程池创建
int threadpoolStart(pThreadPool_t  threadPool);
//循环发送
int transferFile(int peerFd,  char * fileName);
//接收用户上传的文件
int recvFile(int socketFd);
//超时断开连接 //队列信息中添加exitNode_t结构体数组
//全局变量location设置定时器 1s触发一次epoll_wait
//建立一个数组存储要退出的进程的pid 数组下标为每个子线程的fd
//触发epoll_wait时遍历数组检测是有要退出的pid
int addTimeFd(int *timeFd); 
int slot;// 检测socketFd所在的位置


#endif

