#include <func.h>

typedef struct sharedRes_s{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int saletickets;//可出售的票数
    int totaltickets;//库存票的总数
    int soldtickets;//已出售的票数
}sharedRes_t;

void *sellTicket(void *arg){
    sleep(1);
    sharedRes_t *p = (sharedRes_t *)arg;
    int ret;
    while(1){
        ret = pthread_mutex_lock(&p->mutex);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_lock");
        if(p->saletickets > 0)
        {
            --p->saletickets;//手上售票
            ++p->soldtickets;//已经出售的票数    
            if(p->saletickets%50 == 0){
                printf("sellsub saletickets %d\n",p->saletickets);
            }
            if(p->soldtickets%50 == 0){
                printf("sellsub soldtickets %d\n",p->soldtickets);
            }
            if(p->soldtickets == 1000)//已经出售的总票数
            {
                ret = pthread_mutex_unlock(&p->mutex);
                THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
                pthread_exit(NULL);
            }
            if(p->saletickets == 0)
            { //可出售票数
                ret = pthread_cond_signal(&p->cond);
                THREAD_ERROR_CHECK(ret,"pthread_cond_signal");
            }//通知加票
            ret = pthread_mutex_unlock(&p->mutex);    
            THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
            usleep(20000);
        }
        else
        {
            ret = pthread_mutex_unlock(&p->mutex);
            THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
            break;
        }
    }
    pthread_exit(NULL);
}

void *setTicket(void *arg){

    sharedRes_t *p = (sharedRes_t *)arg;
    int ret;
    while(1){
        ret = pthread_mutex_lock(&p->mutex);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_lock");
    //    if(p->totaltickets == 0){
      //      ret = pthread_mutex_unlock(&p->mutex);
        //    THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
          //  pthread_exit(NULL);
       // }
        if(p->saletickets > 0)
        {
            //ret = pthread_mutex_unlock(&p->mutex);
            //THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
            int ret =  pthread_cond_wait(&p->cond,&p->mutex);
            THREAD_ERROR_CHECK(ret,"pthread_cond_wait");
        }
        p->saletickets += 100;//可出售票数
        p->totaltickets -= 100;//库存票数
        printf("sub totaltickets: %d\n", p->totaltickets);
        if(p->totaltickets == 0){
            ret = pthread_mutex_unlock(&p->mutex);
            THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
            pthread_exit(NULL);
        }
        ret = pthread_mutex_unlock(&p->mutex);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_unlock");
    }
    pthread_exit(NULL);
}


int main()
{
    int ret;
    sharedRes_t sharedRes;
    sharedRes.saletickets = 0;//可出售
    sharedRes.totaltickets = 1000;//库存票数
    sharedRes.soldtickets = 0;
    ret = pthread_mutex_init(&sharedRes.mutex,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_init");

    ret = pthread_cond_init(&sharedRes.cond,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_cond_init");

    pthread_t sellpthid1,sellpthid2,setpthid;
    ret = pthread_create(&setpthid,NULL,setTicket,(void *)&sharedRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");

    ret = pthread_create(&sellpthid1,NULL,sellTicket,(void *)&sharedRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");


    ret = pthread_create(&sellpthid2,NULL,sellTicket,(void *)&sharedRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");


    pthread_join(sellpthid1,NULL);
    pthread_join(sellpthid2,NULL);
    pthread_join(setpthid,NULL);

    printf("main thread soldtickets: %d\n",sharedRes.soldtickets);
    ret = pthread_mutex_destroy(&sharedRes.mutex);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_destroy");

    ret = pthread_cond_destroy(&sharedRes.cond);
    THREAD_ERROR_CHECK(ret,"pthread_cond_destroy");
    return 0;
}


