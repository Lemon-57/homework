#include"TaskQueue.h"

//初始化队列的大小
TaskQueue::TaskQueue(size_t sz)
:_queSize(sz)
,_que()
,_mutex()
,_notEmpty(_mutex)
,_notFull(_mutex)
,_isExitFlag(false)
{

}

TaskQueue::~TaskQueue(){

}

bool TaskQueue::empty(){
    return _que.size() == 0;
}

bool TaskQueue::full(){
    return _que.size() == _queSize;
}
//任务入队
void TaskQueue::push(Elemtype value)
{
    MutexLockGuard autoLock(_mutex);
    while (full())//this->full
    {
        _notFull.wait();
    }
    _que.push(value);
    _notEmpty.notify();//唤醒卡在pthread_cond_wait的线程
}
//任务出队
Elemtype TaskQueue::pop(){
    MutexLockGuard autoLock(_mutex);
    while (!_isExitFlag && empty())//标志位为false 队列为空 进入等待
    {                               //防止唤醒后标志位为 false 不会再次卡住 
        _notEmpty.wait();
    }
    if(_isExitFlag){
        Elemtype task = _que.front(); //取出队头元素
        _que.pop();
        _notFull.notify();//唤醒卡在pthread_cond_wait的线程 任务队列此时不满可以取任务了
        return task;
    }else{
        return nullptr;
    }
}
