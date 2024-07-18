#include <func.h>
#include"transferFd.h"
#include"threadPool.h"

void sigfunc(int sigNum){
    printf("sig %d is coming\n", sigNum);
    exit(0);
}

int main()
{
    //ip, port, workerNum指定
    signal(SIGUSR1, sigfunc);//10号信号
    //工作线程数
    int threadNum = 3;
    //三次握手
    int sockFd;
    char ip[] = "127.0.0.1";
    char port[] = "8888";
    tcpInit(ip, port, &sockFd);
    //创建线程池
    threadPool_t threadPool;
    memset(&threadPool, 0, sizeof(threadPool));
    threadpoolInit(&threadPool, threadNum);//初始化线程池
    threadpoolStart(&threadPool);//启动线程池
    //记录将要退出的线程池
    exitNode_t exitNode[20];
    for(int i = 0; i < 20; ++i){
        exitNode[i].exitFd = -1;
        exitNode[i].exitFlag = -1;
        exitNode[i].pid = -1;
    }
    //将队列中exitPipe添加到socketpair
    int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, threadPool.queue.exitPipe);
    ERROR_CHECK(ret, -1, "socketpair");

    int timeFd = 0;
    addTimeFd(&timeFd);
    
    uint64_t exp;
    //创建epoll的实例, 并注册(监听)相应文件描述符上的事件
    int epfd = epollCtor();
    epollAddFd(epfd, sockFd);
    epollAddFd(epfd, threadPool.queue.exitPipe[0]);
    //epollAddTimeFd(epfd, timeFd);//边缘触发监听
    int listenSize = threadNum + 2;
    struct epoll_event * readyList = (struct epoll_event *)calloc(listenSize, sizeof(struct epoll_event));

    int readyNum;
    int newFd;
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
                    newFd = accept(sockFd, NULL, NULL);
                    if(newFd <  0){
                        perror("accept");
                    }
                    printf("accept a new conn, peerFd: %d\n", newFd);
                    //让peerFd入队
                    taskEnqueue(&threadPool.queue, newFd);
                }//退出管道事件就绪
                else if(readyList[i].data.fd == timeFd){
                    read(readyList[i].data.fd, &exp, sizeof(exp));
                    ++slot;
                    slot %= 30;//30s轮转
                    printf("slot down %d\n", slot);
                    for(int j = 0; j < 20; ++j){
                        if(exitNode[j].exitFlag == slot){
                            printf("pid = %d is close\n", exitNode[j].pid);
                            close(exitNode[j].exitFd);
                            kill(exitNode[j].pid, SIGUSR1);
                            //复原pid、exitFd、exitFlag的值，已防重复
                            exitNode[j].exitFd = -1;
                            exitNode[j].exitFlag = -1;
                            exitNode[j].pid = -1;
                        }
                    }
                }
                else if(readyList[i].data.fd == threadPool.queue.exitPipe[0]){
                    read(threadPool.queue.exitPipe[0], exitNode, sizeof(exitNode));
                }
            }
        }
    }
    free(readyList);
    close(sockFd);
    close(epfd);
    return 0;
}
