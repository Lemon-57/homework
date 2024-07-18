#pragma once


#include"Task.h"
#include"Condition.h"
#include<queue>

using std::queue;

using Elemtype = Task *;

class TaskQueue
{
public:
    TaskQueue(size_t sz);//初始化队列的大小
    ~TaskQueue();
    bool empty();
    bool full();
    void push(Elemtype value);//任务入队
    Elemtype pop();//任务出队
private:
    size_t _queSize;
    queue<Elemtype>_que;
    MutexLock _mutex;
    Condition _notEmpty;//这里通过条件变量来进行wait和唤醒
    Condition _notFull;
    bool _isExitFlag;
};

