#pragma once

#include"MutexLock.h"

class Condition
:public NonCopyable//条件变量是临界条件所以和锁一样也是禁止复制和赋值
{
public:
    Condition(MutexLock &mutex);//
    ~Condition();
    void wait();
    void notify();//至少一个线程被唤醒
    void notifyAll();//唤醒所有线程
private:
    MutexLock &_mutex;//防止拷贝
    pthread_cond_t _cond;
};

