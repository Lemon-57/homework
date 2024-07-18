#include"taskQueue.h"

//初始化队列
void queueInit(taskQueue_t * que){
    if(que){
        que->pFront = NULL;
        que->pRear = NULL;
        que->queSzie = 0;
        memset(que->exitNode, 0, sizeof(que->exitNode));
        memset(que->exitPipe, 0, sizeof(que->exitPipe));
        memset(que->cwdPipe, 0, sizeof(que->cwdPipe));
    }
    int ret = pthread_mutex_init(&que->mutex, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");

    ret = pthread_cond_init(&que->cond, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");
}

//任务队列销毁
void queueDestroy(taskQueue_t * que){
    if(que){
        int ret = pthread_mutex_destroy(&que->mutex);
        THREAD_ERROR_CHECK(ret, "pthread_mutex_destroy");
        
        ret = pthread_cond_destroy(&que->cond);
        THREAD_ERROR_CHECK(ret, "pthread_cond_destroy");
    }
}

//任务队列判空
int queueIsEmpty(taskQueue_t * que){
    return (que->queSzie == 0);
}

//获取任务队列大小
int getTaskSize(taskQueue_t * que){
    return que->queSzie;
}

//任务入队queInsert
void taskEnqueue(taskQueue_t * que, int newFd){
    //先加锁
    int ret = pthread_mutex_lock(&que->mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_lock");
    //新建任务节点并初始化
    ptask_t pTask = (ptask_t )calloc(1, sizeof(ptask_t));
    pTask->peerFd = newFd;
    //入队
    if(queueIsEmpty(que)){ //队列为空
        que->pFront = que->pRear = pTask;
    }else{
        que->pRear->pNext = pTask;
        que->pRear = pTask;
    }
    ++que->queSzie;
    //解锁
    ret = pthread_mutex_unlock(&que->mutex);
    THREAD_ERROR_CHECK(ret, "pthread_cond_unlock");
    //通知消费者取任务
    ret = pthread_cond_broadcast(&que->cond);
    THREAD_ERROR_CHECK(ret, "pthread_cond_signal");
}
//任务出队 返回0取到任务 在threadFunc里判断//queGet
int taskDequeue(taskQueue_t * que, ptask_t *pGet){
    //加锁
    int ret = pthread_mutex_lock(&que->mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_lock");
    //队列为空且没有收到退出信息时等待
    while(queueIsEmpty(que)){//queSzie = 0; //while防止虚假唤醒
        pthread_cond_wait(&que->cond, &que->mutex);
    }
    *pGet = que->pFront;
    if(getTaskSize(que) > 1){ //队头出队后队列非空
        que->pFront = que->pFront->pNext;
    }
    else{// =1 出队后队列空
        que->pFront = que->pRear = NULL;
    }
    --que->queSzie;
    //解锁
    ret = pthread_mutex_unlock(&que->mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_unlock");
    //返回出队的socketFd给工作线程
    return 0;
}

//唤醒所有线程退出
void queueWakeUp(taskQueue_t * que){
    int ret = pthread_cond_broadcast(&que->cond);
    THREAD_ERROR_CHECK(ret, "pthread_cond_broadcast");
}
