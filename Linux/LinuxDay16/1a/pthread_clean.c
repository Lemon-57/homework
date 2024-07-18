#include<func.h>

void cleanup(void *p){
    free(p);
    printf("I am cleanup\n");
}
void * threadFunc(void *arg){
    void *p = malloc(100000);
    pthread_cleanup_push(cleanup,p);//一定要在cancel点之前push
    printf("I am child thread, tid = %lu\n",pthread_self());
    //pthread_exit((void *)0);//在pop之前exit，cleanup弹栈并被调用
    pthread_cleanup_pop(1);//在exit之后pop cleanup弹栈，如果参数为1被调用
    pthread_exit((void *)0);
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    printf("I am main thread, tid = %lu\n",
    pthread_self());
    void * pret;
    //ret = pthread_cancel(tid);
    //THREAD_ERROR_CHECK(ret,"pthread_cancel");
    ret = pthread_join(tid,&pret);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("thread return = %ld\n", (long) pret);
    return 0;
}
