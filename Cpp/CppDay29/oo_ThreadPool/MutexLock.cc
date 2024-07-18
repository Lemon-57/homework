#include "MutexLock.h"
#include<stdio.h>

MutexLock::MutexLock()//初始化锁
{
    int ret = pthread_mutex_init(&_mutex, nullptr);
    if(ret){
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock(){//销毁锁
    int ret = pthread_mutex_destroy(&_mutex);
    if(ret){
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::lock(){//上锁
    int ret = pthread_mutex_lock(&_mutex);
    if(ret){
        perror("pthread_mutex_lock");
    }
}

void MutexLock::unlock(){//解锁
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret){
        perror("pthread_mutex_unlock");
    }
}
