#include <func.h>

void * threadFunc(void * arg){
    printf("sub thread is running\n");
    printf("sub thread id:%ld\n", pthread_self());
    while(1);
}

int main()
{
    pthread_t pthid;
    int ret = pthread_create(&pthid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    printf("sub thread id:%ld\n", pthid);
    
    pthread_t mainthid = pthread_self();
    printf("main thread id:%ld\n", mainthid);

    while(1);
    return 0;
}

