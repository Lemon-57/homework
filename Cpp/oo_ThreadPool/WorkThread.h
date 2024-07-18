#ifndef __WORKTHREAD_H__
#define __WORKTHREAD_H__

#include "Thread.h"
#include "ThreadPool.h"

class WorkThread
: public Thread
{
public:
    WorkThread(ThreadPool &pool)
    : _pool(pool)
    {

    }

    void run() override
    {
        //线程池交给子线程去执行的任务
        _pool.threadFunc();
    }

    ~WorkThread()
    {

    }

private:
    ThreadPool  &_pool;

};

#endif
