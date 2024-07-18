#ifndef __THREAD_H__
#define __THREAD_H__

#include "NonCopyable.h"
#include <pthread.h>

class Thread
: NonCopyable
{
public:
    Thread();
    virtual ~Thread();
    void start();
    void join();
private:
    static void *threadFunc(void *arg);
    virtual void run() = 0;

private:
    pthread_t _thid;
    bool _isRunning;
};

#endif
