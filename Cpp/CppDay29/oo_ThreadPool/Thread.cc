#include"Thread.h"
#include<stdio.h>

Thread::Thread()
:_thid(0)
,_isRunning(false)
{

}
Thread::~Thread(){
    if(_isRunning){
        pthread_detach(_thid);
    }
}

void Thread::start(){
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret){
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}
//等待线程结束并回收资源
void Thread::jion(){ 
    int ret = pthread_join(_thid, nullptr);
    if(ret){
        perror("pthread_create");
        return;
    }
    _isRunning = false; //线程结束重新设为false
}
//执行线程任务
void * Thread::threadFunc(void * arg){
    Thread *pth = static_cast<Thread *>(arg);
    if(pth){//pth不为空执行run方法
        pth->run();
    }
}


