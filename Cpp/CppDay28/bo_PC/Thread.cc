#include"Thread.h"
#include<stdio.h>


Thread::Thread() 
:_thid(0)
,_isRunning(false)
{

}

Thread::~Thread(){
    if(_isRunning){
        //让子线程与主线程分离，子线程自己回收自己的资源
        pthread_detach(_thid);
    }
}

void Thread::start(){
    //threadFunc作为pthread_create的第三个参数，必须设为static成员函数，否则传递时会传递隐含的this指针
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret){
        perror("thread start");
        return;
    }
    _isRunning = true;
}

void Thread::join(){
    if(_isRunning){
        int ret = pthread_join(_thid, nullptr);
        if(ret){
            perror("thread join");
            return;
        }
        _isRunning = false;
    }
}
//
void *Thread::threadFunc(void * arg){
    //Thread * this---> void * arg ---> Thread * pth;
    Thread *pth = static_cast<Thread *>(arg);
    if(pth){
        //执行相对应的任务
        pth->run();
    }
    //任务结束退出子线程
    pthread_exit(nullptr);
}


