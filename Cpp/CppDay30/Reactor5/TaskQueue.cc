#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t sz)
: _queSize(sz)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{

}

TaskQueue::~TaskQueue()
{

}

bool TaskQueue::empty() const
{
    return 0 == _que.size();
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}
//任务入队
void TaskQueue::push(ElemType &&value)
{   //先需要进行上锁 _mutex.lock();
    MutexLockGuard autoLock(_mutex);//栈对象autoLock

    //防止虚假唤醒 //pthread_cond_wait可能唤醒多个线程，需要进行再次判断
    /* if(full()) */
    while(full())//this->full
    {
        //等待在满的线程，需要进行wait
        _notFull.wait();
    }

    //只有queue不为满的时候，才可以去进行push数据
    _que.push(std::move(value));

    //此时可以唤醒等待在条件变量为空的线程，唤醒卡在pthread_cond_wait的线程
    _notEmpty.notify();
}

ElemType TaskQueue::pop()
{
    //为了让锁能够自动的销毁，所以可以采用RAII的思想
    MutexLockGuard autoLock(_mutex);//栈对象autoLock

    //_flag标志位
    while(_flag && empty())//标志位为ture 队列为空 进入等待
    {                     
        _notEmpty.wait();//卡主的
    }

    if(_flag)
    {
        ElemType tmp = _que.front();//取出队头元素
        _que.pop();

        _notFull.notify();//唤醒卡在pthread_cond_wait的线程 任务队列此时不满可以取任务了
        return tmp;
    }
    else
    {
        return nullptr;
    }


}
void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}

