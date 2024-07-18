#include "tcp.h"


int main(int argc, char *argv[])
{
    
    ARGS_CHECK(argc,3);
    signal(SIGPIPE,SIG_IGN);//忽略掉SIGPIPE信号不处理    
    
    int listenfd = tcpServerInit(argv[1],atoi(argv[2]));    

    //int listenfd = socket(AF_INET,SOCK_STREAM,0);//创建接听套接字
    //ERROR_CHECK(listenfd, -1,"socket");

    //struct sockaddr_in serAddr;
    //memset(&serAddr, 0, sizeof(serAddr));
    //serAddr.sin_family = AF_INET;//固定值，使用IPv4地址
    //serAddr.sin_addr.s_addr = inet_addr(argv[1]);//传入点分十进制字符串网络主机地址，inet_addr需要转换为网络字节序二进制
    //serAddr.sin_port = htons(atoi(argv[2]));//端口号大端转小端

    //对套接字进行属性设置：网络地址可以重用
    //int reuse = 1;
    //int ret = setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    //ERROR_CHECK(ret,-1,"setsockopt");

    //绑定网络地址
    //ret = bind(listenfd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    //ERROR_CHECK(ret,-1,"bind");

    //进行监听
    //ret = listen(listenfd,10);
    //ERROR_CHECK(ret,-1,"listen");

    //ERROR_CHECK(ret, -1,"accept");

    fd_set rdset;//保存全连接队列里的文件描述符
    fd_set needMonitorSet; //记录需要监听的文件描述符集合
    FD_ZERO(&needMonitorSet);
    //FD_SET(STDIN_FILENO,&needMonitorSet);
    FD_SET(listenfd,&needMonitorSet);//将监听套接字加入集合，监听多个客服端
    int ret;
    int connfds[100] = {0};//保存所有已经建立好的TCP连接
    int conncounts = 0;//记录连接数量
    char buf[1024] = {0};//存储发生读取的内容 应用层缓冲区
    
    time_t rawtime;
    struct tm *info;
    while(1){
        FD_ZERO(&rdset);//清空所有fd
        memcpy(&rdset,&needMonitorSet,sizeof(fd_set));//将就绪的文件描述符放入监听集合中（select会改变集合）
        int readyNum = select(conncounts+4,&rdset,NULL,NULL,NULL);//+4：+1socket创建的文件描述符，+1监听使用的描述符，+1accept从全连接队列中接收使用的文件描述符，+1让OS知道从0-nfds中间找，提高效率
        ERROR_CHECK(readyNum,-1,"select");//rdset保存就绪的描述符
        

        if(FD_ISSET(listenfd,&rdset)){//如果有新连接到来
            int serverFd = accept(listenfd,NULL,NULL);//从全连接队列取得客户端 
            ERROR_CHECK(serverFd,-1,"accept");
            printf("connfds accept %d\n",serverFd);
            
            connfds[conncounts++] = serverFd;
            FD_SET(serverFd,&needMonitorSet);//将新连接的fd放入监听集合中
        }
        for(int i = 0; i < conncounts; i++){//遍历所有人
            if(FD_ISSET(connfds[i],&rdset)){
                memset(buf,0,sizeof(buf));
                ret = recv(connfds[i],buf,sizeof(buf),0);
                if(ret > 0){ //>0 表示从对端接收了ret个字节的数据
                    for(int j = 0; j < conncounts;j++)//给所有人发消息
                    {
                        if(connfds[i] == connfds[j]){ //fd相同为自己跳过
                            continue;
                        }else{
                            ret = send(connfds[j],buf,strlen(buf),0);
                        }
                        
                    }
                }
                else if(0 == ret){ // =0 表示连接已经断开
                    printf("%d connfds closed\n",connfds[i]);
                    close(connfds[i]);
                    FD_CLR(connfds[i],&needMonitorSet);//删除断开连接的文件描述符
                }
                else if(-1 == ret && errno == EINTR){
                    continue;
                }
                else{
                    ERROR_CHECK(ret,-1,"recv");
                }
            }
        }
    }
    for(int i = 0;i < conncounts;i++){
        //printf("%d close\n",connfds[i]);
        close(connfds[i]);
    }
    close(listenfd);

    return 0;
}

