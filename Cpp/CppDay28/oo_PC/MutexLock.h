#pragma once

#include "NonCopyable.h"
#include <pthread.h>


class MutexLock
:NonCopyable
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t * getMutexLockPtr(){
        return &_mutex;
    }
    /* MutexLock(const MutexLock &rhs) = delete; */
    /* MutexLock &operator=(const MutexLock &rhs) = delete; */

private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard{
public:
    MutexLockGuard(MutexLock &mutex)
    :_mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;
};
