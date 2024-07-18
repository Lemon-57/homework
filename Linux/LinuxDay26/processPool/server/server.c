#include <func.h>
#include"worker.h"
#include"transferFd.h"
#include"tcp_epoll.h"

int exitPipeFd[2];

void sigFunc(int sigNum){
    printf("sigNum = %d\n", sigNum);
    //通过单独的管道通知父进程退出
    write(exitPipeFd[1], "1", 1);
}

int main(int argc, char *argv[])
{
    //SIGUSR1表示用户自定义的信号
    signal(SIGUSR1,sigFunc);
    //ip, port, workerNum
    ARGS_CHECK(argc, 4);
    int workerNum = atoi(argv[3]);
    //子进程信息记录数组
    processData_t *workerList = (processData_t *)calloc(workerNum, sizeof(processData_t));
    //创建processNum个管道和子进程  
    makeChild(workerList, workerNum);
    //创建TCP的监听套接字listenfd
    int sockFd;
    tcpInit(argv[1], argv[2], &sockFd);
    //创建epoll的实例, 并注册(监听)相应文件描述符上的事件
    int epfd = epollCtor();
    epollAdd(sockFd, epfd);
    for(int i = 0; i < workerNum; ++i){
        epollAdd(workerList[i].pipeFd, epfd);
        printf("%d\n", workerList[i].pipeFd);
    }
    //创建退出的管道,并注册(监听)
    pipe(exitPipeFd);
    epollAdd(exitPipeFd[0], epfd);
    
    int listenSize = workerNum + 2;
    
    struct epoll_event * readyList = (struct epoll_event *)calloc(listenSize, sizeof(struct epoll_event));
    
    int readyNum;
    while(1){
        do{
            //等待epoll事件就绪
            readyNum = epoll_wait(epfd, readyList, listenSize, -1);
        }while(-1 == readyNum && errno == EINTR);
        
        if(0 == readyNum){
            printf("epoll timeout\n");
            continue;
        }else if(-1 == readyNum){
            perror("epoll_wait");
            return EXIT_FAILURE;
        }else{//readyNum > 0
            for(int i = 0; i < readyNum; ++i){
                //客户端到来时的处理
                if(readyList[i].data.fd == sockFd){
                    printf("accept ready\n");
                    int peerFd = accept(sockFd, NULL, NULL);
                    for(int j = 0; j < workerNum; ++j){
                        //查找一个空闲的进程，发送给子进程处理
                        if(workerList[j].status == FREE){
                            printf("%d worker is BUSY, pid = %d\n", j, workerList[j].pid);
                            sendFd(workerList[j].pipeFd, peerFd, 0);
                            workerList[j].status = BUSY;
                            break;
                        }
                    }
                    close(peerFd);
            
                }//退出管道事件就绪
                else if(readyList[i].data.fd == exitPipeFd[0]){
                    int exitFlag;
                    read(exitPipeFd[0], &exitFlag, 1);
                    //父进程给所有工作进程发终止信息
                    for(int j = 0; j < workerNum; ++j){
                        printf("worker exit\n");
                        //不需要传递传递文件描述符，第二个参数设置为0
                        //第三个参数是子进程退出的标志位  1退出进程
                        printf("%d\n", workerList[j].pipeFd);
                        sendFd(workerList[j].pipeFd, 0, 1);
                    }
                    //回收子进程的资源，不回收变成僵尸进程
                    for(int j = 0; j < workerNum; ++j){
                        wait(NULL);
                    }
                    puts("parent process exit");
                    exit(0);
                }
                else{
                    //子进程任务执行完毕，通知父进程
                    ////父进程收到通知(某一个管道有数据过来)时，将子进程标志为空闲
                    puts("worker finish work");
                    for(int j = 0; j < workerNum; ++ j){
                        pid_t pid;
                        int ret = recv(workerList[j].pipeFd, &pid, sizeof(pid_t), 0);
                        printf("%d worker is FREE\n", workerList[j].pid);
                        workerList[j].status = FREE;
                        break;
                    }
                }
            }
        }

    }
    free(workerList);
    close(sockFd);
    close(epfd);
    return 0;
}

