#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include "Task.h"
#include <queue>

using std::queue;

using ElemType = Task *;

class TaskQueue
{
public:
    TaskQueue(size_t sz);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(ElemType value);
    ElemType pop();

    /* void wakupAllNotEmpty(); */
    void wakeup();

private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;

};

#endif
