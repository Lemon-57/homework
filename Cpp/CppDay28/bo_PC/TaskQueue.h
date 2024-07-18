#pragma once

#include "MutexLock.h"
#include "Condition.h"
#include <queue>

using std::queue;

class TaskQueue
{
public:
    TaskQueue(size_t sz);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(const int &value);
    int pop();
private:
    size_t _queueSize;
    queue<int> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
};

