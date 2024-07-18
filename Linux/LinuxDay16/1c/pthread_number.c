// 尝试使用2个线程对同一个全局变量各加2000万。
#include <func.h>

int global = 0;

void *threadFunc(void *arg){        
    printf("sub global behind:%d\n",global);
    global+=20000000;
    printf("sub global before:%d\n",global);
}
int main(){
    pthread_t tid;
    printf("main global before: %d\n",global);
    int ret = pthread_create(&tid,NULL,threadFunc,NULL);
//    sleep(1);
    global+=20000000;
    printf("main global behind:%d\n",global);
    pthread_cancel(tid);
  //  printf("main sub global behind:%d\n",global);
    pthread_join(tid,NULL);
    return 0;
}

