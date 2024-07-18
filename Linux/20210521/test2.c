#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void* threadFun(void* arg)
{
    printf("child thread\n");
}

int main()
{
    pthread_t t;
    if(pthread_create(&t, NULL, threadFun, NULL) != 0) {
        fprintf(stderr, "create thread fail\n");
        exit(-1);
    }   

    if(pthread_detach(t) != 0) {
        fprintf(stderr, "thread join fail\n");
        exit(-1);
    }   

    printf("child thread exit\n");
    sleep(1);

    exit(0);
}