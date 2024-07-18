#include <func.h>

void *start_routine(void *arg){
    printf("I am child thread, tid = %lu\n",
           pthread_self());
}


int main()
{
    printf("I am main thread, tid = %lu\n",
           pthread_self());
    pthread_t tid;
    int ret;
    ret = pthread_create(&tid,NULL,start_routine,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    usleep(20);
    return 0;
}

