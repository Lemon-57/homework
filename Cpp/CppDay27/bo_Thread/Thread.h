#pragma once

#include<pthread.h>
#include<functional>
using std::function;

using ThreadCallback = function<void()>;

class Thread
{
public:
    Thread(ThreadCallback &&cb);
    virtual ~Thread();
    void start();
    void join();
private:
    static void * threadFunc(void * arg);
    // virtual void run() = 0;
private:
    pthread_t _thid;
    bool _isRunning;
    ThreadCallback _cb;
};

