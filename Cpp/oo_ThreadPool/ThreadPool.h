#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Thread.h"
#include "TaskQueue.h"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

/* class WorkThread; */

class ThreadPool
{
    friend class WorkThread;
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start();//开启线程池
    void stop();//结束线程池

    //添加与获取任务
    void addTask(Task *ptask);
    Task *getTask();

private:
    void threadFunc();//交给子线程做的任务

private:
    size_t _threadNum;//子线程的数目
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;//存放子线程的容器
    TaskQueue _taskQue;
    bool _isExit;
};

#endif
