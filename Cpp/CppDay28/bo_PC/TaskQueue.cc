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

void TaskQueue::push(const int &value){
    
} 

int TaskQueue::pop(){

}

