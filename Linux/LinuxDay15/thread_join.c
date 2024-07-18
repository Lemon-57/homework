#include <func.h>
void * threadFunc(void * arg){
    printf("sub thread is running\n");
    printf("sub thread id:%ld\n", pthread_self());
    int *pNumber = (int*)calloc(1,sizeof(int));
    *pNumber = 10;
    printf("sub thread pNumber:%p\n",pNumber);
    return pNumber;
}
int main()
{
    pthread_t pthid;
    int ret = pthread_create(&pthid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    printf("sub thread id:%ld\n", pthid);
    pthread_t mainthid = pthread_self();
    printf("main thread id:%ld\n", mainthid);
    int *p = NULL;  
    pthread_join(pthid,(void**)&p);
    printf("main thread p:%p *p=%d\n",p,*p);
    return 0;
}

