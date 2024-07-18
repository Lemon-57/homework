#include<func.h>

enum workerStatus{
    FREE,// 0
    BUSY //自动赋值1
};

typedef struct{
    pid_t pid;
    int status;
    int pipeFd;
}processData_t,*pProcessPool_t;

typedef struct train_s{
    int dataLenth;
    char buf[1024];
}train_t;

int makeChild(processData_t *pProccessData, int processNum);

void handleEvent(int pipeFd);

int sendn(int sockFd, void * buf, int len);

int transFile(int peerFd);
