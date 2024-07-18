#include "TaskQueue.h"


TaskQueue::TaskQueue(size_t sz)
:_queueSize(sz)
,_que()
,_mutex()
,_notEmpty(_mutex)
,_notFull(_mutex)
{

}

TaskQueue::~TaskQueue(){

}

bool TaskQueue::empty() const{
    return 0 == _que.size();
}
bool TaskQueue::full() const{
    return _que.size() == _queueSize;
}

void TaskQueue::push(const int &value){//添加任务
    //先需要进行上锁
    //_mutex.lock();
    MutexLockGuard autoLock(_mutex);//栈对象autoLock
    
    //防止虚假唤醒 //pthread_cond_wait可能唤醒多个线程，需要进行再次判断
    /* if(full()) */
    while (full())
    {
        //等待在满的线程 需要进行wait
        _notFull.wait();
    }
    //只有当queue不为满的时候，才可以进行push数据
    _que.push(value);
    //...return
    //if(...)
    //{
    //   return;
    //}

    //此时可以唤醒等待在条件变量为空的线程
    _notEmpty.notify();


} 

int TaskQueue::pop(){
    //为了让锁能够自动的销毁，所以可以采用RAII的思想
    //
    /* _mutex.lock */
    MutexLockGuard autoLock(_mutex);//栈对象autoLock
    /* if(empty()){} */
    while (empty())
    {
        _notEmpty.wait();
    }
    int tmp = _que.front();
    _que.pop();
    //...
    //return;

    _notEmpty.notify();
    
    //_mutex.unlock();
    return tmp;
}

