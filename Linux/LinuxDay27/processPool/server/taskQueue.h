#ifndef _TASKQUEUE_H_
#define _TASKQUEUE_H_

#include<func.h>
//0不退出 1退出
#define TASKQUEUE_NOT_EXIT_FLAG 0
#define TASKQUEUE_READY_EXIT_FLAG 1

//任务(task)队列节点
typedef struct task_s{
    int peerFd;//与客户端相连的sockeFd
    struct task_s *pNext;
}task_t, *ptask_t;

typedef struct exitNode_s{
    int exitFd;//记录要退出的线程fd
    int pid;//记录要退出客户端的pid，以便退出时kill掉
    int exitFlag;//设置为各子线程的退出标志 
}exitNode_t, *pexitNode_t;

//任务队列链表
typedef struct taskQueue_s{
    ptask_t pFront;
    ptask_t pRear;
    int queSzie;//当前任务的个数
    pthread_mutex_t mutex;//任务队列的锁
    pthread_cond_t cond;
    int exitPipe[2];//通过管道通知父进程退出
    int cwdPipe[2];//标记当前目录 current working directory
    exitNode_t exitNode[20];//存储要退出进程的pid，数组下标为每个子线程的fd
}taskQueue_t, *ptaskQueue_t;

//初始化队列
void queueInit(taskQueue_t * que);
//任务队列销毁
void queueDestroy(taskQueue_t * que);
//任务队列判空
int queueIsEmpty(taskQueue_t * que);
//获取任务队列大小
int getTaskSize(taskQueue_t * que);
//任务入队
void taskEnqueue(taskQueue_t * que, int newFd );
//取任务出队
int taskDequeue(taskQueue_t * que, ptask_t *pGet);
//唤醒所有线程退出
void queueWakeUp(taskQueue_t *);


#endif
