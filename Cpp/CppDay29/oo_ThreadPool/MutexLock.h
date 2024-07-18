#pragma once

#include "NonCopyable.h"
#include <Thread.h>

class MutexLock
:NonCopyable //因为只有一把锁所以需要禁止赋值和复制
{
public:
    MutexLock();//初始化锁
    ~MutexLock();//销毁锁
    void lock();//上锁
    void unlock();//解锁
    //获取锁的地址 因为创建的是类对象，如果直接对类对象取址，获取的是对象的地址而不是类中锁的地址
    pthread_mutex_t * getMutexLockPtr(){
        return &_mutex;
    }
private:
    pthread_mutex_t  _mutex;
};

class MutexLockGuard//使用MutexLockGuard代替MutexLock来达到锁的自动释放 类似RAII技术
{
public:
    MutexLockGuard(MutexLock &mutex)//&mutex 防止调用拷贝构造函数 
    :_mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;//这里传引用防止拷贝
};