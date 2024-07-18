#include "tcp.h"

#define MAXEVNTS 100

int main(int argc, char *argv[])
{
    int channelsize = 0;//当前已经建立好的通道数
    channel_t channels[MAXEVNTS] = {0};//创建通道数组
    channelsInit(channels,MAXEVNTS);//初始化数组

    ARGS_CHECK(argc,3);
    signal(SIGPIPE,SIG_IGN);//忽略掉SIGPIPE信号不处理    
    
    int listenfd = tcpServerInit(argv[1],atoi(argv[2]));
    setNonblock(listenfd);

    int epfd = epoll_create1(0); 
    ERROR_CHECK(epfd, -1, "epoll_create1");
    
    struct epoll_event * pevtList =(struct epoll_event *)calloc(MAXEVNTS,sizeof(struct epoll_event));
    //存储已就绪的文件描述符    
    
    epollAddReadEvent(epfd,listenfd);
    int ret;
    int connfds[100] = {0};//保存所有已经建立好的TCP连接
    int conncounts = 0;//记录连接数量
    char buf[1024] = {0};//存储发生读取的内容 应用层缓冲区
    
    struct tm *info;
    while(1){
        ret = epoll_wait(epfd,pevtList,MAXEVNTS, -1);
        if(0  == ret){
            printf("epoll_wait timeout. \n");
            for(int i = 0; i < conncounts;i++){
                printf("%d connfds closed\n",connfds[i]);
                close(connfds[i]);
            }
        }else if(-1 == ret && errno == EINTR){
            continue;
        }else if(-1 == ret){
            perror("epoll_wait");
        }
        else{//ret > 0 ret已经就绪的fd数量
            for(int i = 0; i < ret; i++){
                int fd = pevtList[i].data.fd;//要关注的fd
                if(fd == listenfd){//新连接到来    
                    int serverFd = accept(listenfd,NULL,NULL);//从全连接队列取得客户端 
                    ERROR_CHECK(serverFd,-1,"accept");
                    printf("connfds accept %d\n",serverFd);
                    connfds[conncounts++] = serverFd;
                    //将serverfd加入到监听队列
                    epollAddReadEvent(epfd,serverFd);

                }else{//已经建立好的连接
                     if(pevtList[i].events & EPOLLIN){
                        memset(buf,0,sizeof(buf));
                        int nret = recv(fd,buf,sizeof(buf),0);
                        if(nret > 0){
                            for(int j = 0;j < 100; j++){
                                if(connfds[j] == fd){
                                    continue;       
                                }else{
                                    //printf("%d ,%d\n",connfds[j],j);
                                    nret = send(connfds[j],buf,strlen(buf),0);    
                                }
                            }
                        }else if(0 == nret){
                           // printf("%d connfds closed\n",connfds[i]); 
                            //从监听队列里移除
                            epollDelEvent(epfd,fd);
                        }else if(-1 == nret && errno == EINTR){
                                continue;
                        }
                        else{
                            ERROR_CHECK(nret, -1, "recv");
                        }

                    }
                }
            }
            
        }
       
    }
    for(int i = 0;i < conncounts;i++){
        close(connfds[i]);
    }

    return 0;
}

