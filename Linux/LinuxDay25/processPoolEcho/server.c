#include"tcp.h"
#include"childProcess.h"
#include"epoll.h"
int main(int argc,char *argv[])
{
    // ./main ip port 管道组个数
    ARGS_CHECK(argc,4);
    signal(SIGPIPE,SIG_IGN);//忽略SIGPIPE信号不处理
    int workerNum = atoi(argv[3]);
    //1.初始化大小为workerNum的管道数组
    processData_t *workerList = (processData_t*)calloc(workerNum,sizeof(processData_t));    
    makeChild(workerList,workerNum);
    //2.创建listen套接字，等待客户端的到来
    int listenfd = tcpServerInit(argv[1],atoi(argv[2]));
    //3.创建epoll实例，并注册相应的fd
    int epfd = epollCreate();
    //监听socket读事件
    epollAddReadEvent(epfd, listenfd);
    //监听管道每个子进程的读事件，
    for(int i = 0; i < workerNum; i++){
        epollAddReadEvent(epfd, workerList[i].pipeFd);
    }
    int listenSize = workerNum+1;//socket+每个进程pipe的读端
    //存储已经就绪文件描述符链表
    struct epoll_event * readyList = (struct epoll_event*)
        calloc(listenSize, sizeof(struct epoll_event));
    int readyNum;
    while(1){
       // do{
            //4.epoll等待事件就绪
            readyNum = epoll_wait(epfd,readyList,listenSize,-1);
   //     } while(-1 == readyNum && errno == EINTR);

   //     if(0 == readyNum){
   //         printf("epoll_wait timout");
   //         continue;
   //     }else if(-1 == readyNum){
   //         perror("epoll_wait");
    //        return EXIT_FAILURE;
     //   }else{//readyNum > 0
             for(int i = 0; i < readyNum; ++i){
                 if(readyList[i].data.fd == listenfd){//5.等待客户端到来的处理
                    int peerFd = accept(listenfd,NULL,NULL); 
                    for(int j = 0; j < workerNum; ++j){
                        if(workerList[j].status == FREE){
                            //5.1查找一个空闲的进程，发送peerFd，交给子进程处理
                            printf("%d worker go to work, pid = %d\n",j,workerList[j].pid); 
                            sendFd(workerList[j].pipeFd,peerFd);
                            workerList[j].status = BUSY;                    
                            break;
                        }
                    }
                    close(peerFd);//父进程交给子进程一定要关闭
                 }
                 else{
                     printf(" worker finish work" );
                     int j;
                     for(j = 0; j < workerNum;++j){
                         if(workerList[j].pipeFd == readyList[i].data.fd){
                             pid_t pid;
                             int ret = recv(workerList[j].pipeFd, &pid, sizeof(pid_t), 0);
                             printf("%d worker not BUSY, pid = %d\n",j, pid);
                             workerList[j].status = FREE;
                             break;
                         }
                     }

                 }
            // }
        }

    }
     return 0;
}

