#include <func.h>

typedef struct sharedRes_s{
    int val;
    pthread_mutex_t mutex;
}sharedRes_t;

void * threadFunc(void *arg){
    sharedRes_t *p = (sharedRes_t*)arg;
    int cnt = 10000000;
    int ret;
    while(cnt-- > 0){
        ret = pthread_mutex_lock(&p->mutex);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_lock");
        ++ p->val;
        ret = pthread_mutex_unlock(&p->mutex);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
    }
    pthread_exit(NULL);
}

int main()
{
    int ret;
    pthread_t sub1,sub2;
    sharedRes_t sharedRes;
    sharedRes.val = 0;
    ret = pthread_mutex_init(&sharedRes.mutex,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_init");
    
    struct timeval begTime,endTime;
    gettimeofday(&begTime,NULL);
    
    ret = pthread_create(&sub1,NULL,threadFunc,(void*)&sharedRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    
    ret = pthread_create(&sub2,NULL,threadFunc,(void*)&sharedRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    
    pthread_join(sub1,NULL);
    pthread_join(sub2,NULL);
    
    ret = pthread_mutex_destroy(&sharedRes.mutex);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_destroy");
    gettimeofday(&endTime,NULL);
    long usecTime = (endTime.tv_sec - begTime.tv_sec)*10000000+(endTime.tv_usec - begTime.tv_usec);
    printf("usetime-usec:%ld\n",usecTime);
    return 0;
}

