#include"childProcess.h"

//初始化管道
int makeChild(processData_t *pProcessData, int processNum){
        pid_t pid;
        for(int i = 0; i < processNum; ++i){
            int pipeFd[2];
            socketpair(AF_LOCAL,SOCK_STREAM,0,pipeFd);
            pid = fork();
            if(pid == 0){//子进程要做的任务
                close(pipeFd[0]);//子进程是要做发送操作，通知父进程任务结束
                handleEvent(pipeFd[1]);//一至循环等待任务
            }
            //父进程记录子进程信息
            close(pipeFd[1]);
            printf("pid = %d, pipeFd[0] = %d\n", pid,pipeFd[0]);
            pProcessData[i],pid = pid;
            pProcessData[i].status = FREE;
        }
    return 0;
}


int handleEvent(int pipeFd){
    int netFd;
    while(1){
        //子进程recv阻塞在pipeFd，当recvFd收到pipeFd时，表示管道中有数据到来
        //子进程从recvmsg中返回，给父进程传输文件
        recvFd(pipeFd,&netFd);//接收管道传输的文件描述符
        char buf[1024] = {0};
        //回显
        recv(netFd,buf,sizeof(buf),0);
        puts(buf);
        send(netFd,"Echo",4,0);
        close(netFd);
        //发送子进程的pid
        pid_t pid = getpid();
        send(pipeFd,&pid,sizeof(pid_t),0); 
    }
}



