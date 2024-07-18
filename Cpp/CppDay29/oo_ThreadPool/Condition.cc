#include"Condition.h"
#include<stdio.h>

Condition::Condition(MutexLock &mutex)
:_mutex(mutex)
{
    int ret = pthread_cond_init(&_cond, nullptr);
    if(ret){
        perror("pthread_cond_init");
    }
}

Condition::~Condition(){
    int ret = pthread_cond_destroy(&_cond);
    if(ret){
        perror("pthread_cond_destroy");
    }
}

void Condition::wait(){
    //获取锁的地址 因为创建的是类对象，如果直接对类对象取址，获取的是对象的地址而不是类中锁的地址
    //所以需要通过成员函数来获取成员的地址
    int ret = pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
    if(ret){
        perror("pthread_cond_wait");
    }
}

//至少一个线程被唤醒
void Condition::notify(){
    int ret = pthread_cond_signal(&_cond);
    if(ret){
        perror("pthread_cond_signal");
    }
}

//唤醒所有线程
void Condition::notifyAll(){
    int ret = pthread_cond_broadcast(&_cond);
    if(ret){
        perror("pthread_cond_wait");
    }
}

