#include"worker.h"
#include"transferFd.h"

//创建processNum个管道和子进程
int makeChild(processData_t *pProcessData, int processNum){
    pid_t pid;
    for(int i = 0; i < processNum; ++i){
        int pipeFd[2];
        socketpair(AF_LOCAL, SOCK_STREAM, 0, pipeFd);
        pid = fork();
        if(0 == pid){//子进程要做的是发送操作，通知父进程任务结束
            close(pipeFd[0]);
            handleEvent(pipeFd[1]);//一直再循环
        }
        //父进程记录子进程的消息
        close(pipeFd[1]);
        printf("pid = %d, pipeFd[0] = %d\n", pid, pipeFd[0]);
        pProcessData[i].pid = pid;
        pProcessData[i].status = FREE;
        pProcessData[i].pipeFd = pipeFd[0];
    }
    return 0;
}

void handleEvent(int pipeFd){
    int netFd;
    while(1){
        printf("handleEvent\n");
        //子进程recv阻塞在pipeFd，当recvFd收到pipeFd时，表示管道中有数据来
        //子进程从recvmsg中返回，给父进程传输文件
        int exitFlag;
        recvFd(pipeFd, &netFd, &exitFlag);////接收管道传输的文件描述符
        printf("%d\n",getpid());
        if(exitFlag == 0){//不退出
            //发送文件
            transFile(netFd);
            close(netFd);
             //发送子进程的pid
            pid_t pid = getpid();
            send(pipeFd, &pid, sizeof(pid_t), 0);
        }else{//eixtFlag = 1
            printf("worker %d is exit\n",getpid());
            puts("worker is  closing");
            exit(0);
        }
    }
}

