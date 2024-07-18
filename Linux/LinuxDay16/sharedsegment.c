//共享数据段

#include <func.h>

int global = 100;

void *threadFunc(void *arg){
    printf("I am child thread, tid = %lu\n",
           pthread_self());
    printf("child thread,global = %d\n",global);
    return NULL;
}


int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,NULL);
  //  THREAD_ERROR_CHECK(ret,"pthread_create");

    return 0;
}

