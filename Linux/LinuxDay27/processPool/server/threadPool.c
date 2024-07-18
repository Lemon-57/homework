#include"threadPool.h"
//线程池
//线程入口函数
void * threadFunc(void * pArg){
    ptaskQueue_t ptaskQueue = (ptaskQueue_t)pArg;//新建任务队列链表
    ptask_t pcurTask = NULL;//新建任务队列节点
    cmdMsg_t cmdMsg;
    char primePath[128] = {0};//起始路径
    char currentPath[128] = {0};//当前路径
    char lastPath[128] = {0};//上一次路径
    //在线程内部获取自己的线程id
    printf("sub thread %ld running\n", pthread_self());
    char * nofile = "no such file or directory";
    while(1){
        //取任务 //任务队列为空就卡在taskDequeue
        int getTask =  taskDequeue(ptaskQueue, &pcurTask);
        if(0 == getTask){//成功取出
            int pid; //记录需要退出的子线程pid
            recv(pcurTask->peerFd, &pid, sizeof(int), 0);//接收ipd
            printf("%d\n", pid);
            char username[64] = {0};
            recv(pcurTask->peerFd, username, sizeof(username), 0);//接收用户名
            userLogin(pcurTask->peerFd, username);//校验username salt
            getcwd(primePath, sizeof(primePath));//获取初始路径
            printf("primePath: %s\n", primePath);
            while(1){
                ptaskQueue->exitNode[pcurTask->peerFd].exitFlag = slot++;
                ptaskQueue->exitNode[pcurTask->peerFd].pid = pid;//记录退出线程的pid，已变退出时kill
                ptaskQueue->exitNode[pcurTask->peerFd].exitFd = pcurTask->peerFd;
                slot %= 30;
                printf("ptaskQueue->exitNode[pcurTask->peerFd].exitFlag = %d\n", ptaskQueue->exitNode[pcurTask->peerFd].exitFlag);
                printf("ptaskQueue->exitNode[pcurTask->peerFd].pid = %d\n", ptaskQueue->exitNode[pcurTask->peerFd].pid);
                printf("ptaskQueue->exitNode[pcurTask->peerFd].exitFd = %d\n", ptaskQueue->exitNode[pcurTask->peerFd].exitFd);
                write(ptaskQueue->exitPipe[1], ptaskQueue->exitNode, 20 *sizeof(exitNode_t));
                //清空消息
                memset(&cmdMsg, 0, sizeof(cmdMsg)); 
                int ret = recv(pcurTask->peerFd, &cmdMsg, sizeof(cmdMsg), 0);//没收到用户信息阻塞
                if(0 == ret || -1 == ret){//消息为空|出错
                    break;
                }
                //收到消息插入数据库消息日志
                writeLog(cmdMsg, username);
                //const char *buf = "no such file";
                switch(cmdMsg.cmd){
                case CD:
                    ret = cdCmd(primePath, lastPath, cmdMsg.data);
                    if(0 == ret){
                        socketpair(AF_UNIX, SOCK_STREAM, 0, ptaskQueue->cwdPipe);
                        write(ptaskQueue->cwdPipe[1], primePath, sizeof(primePath));
                        if(0 == fork()){
                            //创建的进程继承了之前的进程环境，当前目录是家目录
                            char parentPath[128] = {0};
                            read(ptaskQueue->cwdPipe[0], parentPath, sizeof(parentPath));
                            chdir(parentPath);
                            getcwd(parentPath, sizeof(parentPath));//更新工作目录
                            write(ptaskQueue->cwdPipe[0], parentPath, sizeof(parentPath));
                            exit(0);
                        }
                        else{//父进程
                            read(ptaskQueue->cwdPipe[1], primePath, sizeof(primePath));
                            send(pcurTask->peerFd, primePath, sizeof(primePath), 0);
                        }
                    }
                    if(-1 == ret){
                        send(pcurTask->peerFd, nofile, sizeof(nofile), 0);
                    }
                    break;
                case LS:
                    lsCmd(pcurTask->peerFd, primePath);
                    break;
                case PUTS: 
                    putsCmd(pcurTask->peerFd);
                    break;
                case GETS:
                    getsCmd(pcurTask->peerFd, cmdMsg.data);
                    break;
                case PWD:
                    pwdCmd(pcurTask->peerFd, primePath);
                    break;
                case MKDIR:
                    mkdirCmd(primePath, currentPath, cmdMsg.data);
                    break;
                case RMDIR:
                    rmCmd(primePath, currentPath, cmdMsg.data);
                    break;
                default:
                    break;
                }   
            }
        }
    }
    pcurTask = NULL;
    free(pcurTask);
    printf("sub thread %ld is ready to exit\n", pthread_self());
    pthread_exit(0);
}
//线程池初始化
void threadpoolInit(pThreadPool_t threadPool, int threadNum){
    memset(threadPool, 0, sizeof(threadPool_t));
    queueInit(&threadPool->queue);//初始化任务队列
    threadPool->threadFlag = 0;
    threadPool->threadNumber = threadNum;
    threadPool->threads = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
}
//线程池销毁
void threadpoolDestroy(pThreadPool_t threadPool){
    free(threadPool->threads);
    queueDestroy(&threadPool->queue);
}
//线程池创建
int threadpoolStart(pThreadPool_t  threadPool){
    if(0 == threadPool->threadFlag){        
        for(int idx = 0; idx < threadPool->threadNumber; ++idx ){
            int ret = pthread_create(&threadPool->threads[idx], NULL, threadFunc, &threadPool->queue);
            THREAD_ERROR_CHECK(ret, "pthread_create");
        }
        threadPool->threadFlag = 1;
        return 0;
    }
    return -1;
}
