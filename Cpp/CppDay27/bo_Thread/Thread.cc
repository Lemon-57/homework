#include"Thread.h"

//回调函数在此处被注册
Thread::Thread(ThreadCallback &&cb)
:_thid(0)
,_isRunning(false)
,_cb(std::move(cb)) 
{

}
Thread::~Thread(){
    if(_isRunning){
        //让子线程与主线程分离，子线程自己回收自己的资源
        pthread_detach(_thid);
    }
}

void Thread::start(){
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret){
        perror("pthread_cread");
        return;
    }
    _isRunning = true;
}

void Thread::join(){
   if(_isRunning){
        int ret = pthread_join(_thid, nullptr);
        if(ret){
            perror("pthread_jion");
            return;
        }
        _isRunning = false;
   } 
}

void *Thread::threadFunc(void * arg){

    Thread *pth = static_cast<Thread *>(arg);
    if(pth){
        //执行操作 run
        pth->_cb();//执行对应的任务，执行回调函数
    }
    pthread_exit(nullptr);
}


