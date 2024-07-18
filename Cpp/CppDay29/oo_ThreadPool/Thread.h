#pragma once

#include"NonCopyable.h"
#include<Thread.h>


class Thread
{
public:
    Thread();
    virtual ~Thread();
    void start();
    void jion();
private:
    static void * threadFunc(void * arg);
    virtual void run(); //提供给派生类的run方法
private:
    pthread_t _thid;//线程id
    bool _isRunning;//标志位 判断线程是否运行
};

