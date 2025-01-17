#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>

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
    static void *threadFunc(void *arg);//线程入口函数 static为了消除this指针

private:
    pthread_t _thid;
    bool _isRunning;
    ThreadCallback _cb;
};

#endif
