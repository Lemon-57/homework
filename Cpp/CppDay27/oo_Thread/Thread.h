#pragma once

#include <pthread.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();
    void start();//线程是否开始
    void join();//线程是否结束
private:
    static void * threadFunc(void * arg);//线程入口函数 static为了消除this指针
    virtual void run() = 0;//具体需要执行的任务
private:
    pthread_t _thid;
    bool _isRunning;
};

