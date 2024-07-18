#include"EventLoop.h"
#include"Acceptor.h"
#include<unistd.h>
#include<iostream>


using std::endl;
using std::cout;

EventLoop::EventLoop(Acceptor &acceptor)
:_epfd(createEpollFd())
,_acceptor(acceptor)
,_isLooping(false)
,_evtList(1024)
{
    addEpollReadFd(_acceptor.fd());//监听listenfd，是否有客户端发出连接请求
}

EventLoop::~EventLoop(){
    if(_epfd){
        close(_epfd);
    }
}
//开始轮询
void EventLoop::loop(){
    _isLooping = true;
    while (_isLooping)//
    {
        waitEpollFd();
    }
    
}
//关闭轮询
void EventLoop::unloop(){
    _isLooping = false;
}


//EvenLoop只需要将三个事件传递给TcpConnection 
//所以只需要注册，不需要执行， 就不需要对应的handle函数 
void EventLoop::setConnectionCallback(TcpConnectionCallback &&cb){
    _onConnectionCb = std::move(cb);
}
    
void EventLoop::setMessageCallback(TcpConnectionCallback &&cb){
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb){
    _onCloseCb = std::move(cb);
}
//对就绪链表的文件进行轮询是否有I/O事件发送
void EventLoop::waitEpollFd()
{
    int nready = -1;
    do{     //每三秒轮询一次
        nready = epoll_wait(_epfd, & *_evtList.begin(), _evtList.size(), 3000);
    }while(-1 == nready && errno == EINTR); //如果发生中断错误 continue
    
    if(-1 == nready){
        perror("epool_wait nready == -1");
        return;
    }
    else if(0 == nready){
        cout << ">> epoll_wait timeout" <<endl;
    }
    else //ret > 0 
    {  
        if(nready == (int)_evtList.size()){//可以解决扩容问题， 请求连接数超过1024
            _evtList.resize(2 * nready);
        }
        for(int idx = 0; idx < nready; ++idx){
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd())//_acceptor.fd()为新连接的fd
            {
                if(_evtList[idx].events & EPOLLIN)//并且是读事件
                {
                    //puts("5");
                    handleNewConnection();//处理新的连接请求
                }
            }
            else //是就绪队列里的连接有数据发过来了
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleMessage(fd);//进行正常业务逻辑，进行数据的收发
                }
            }
        }
    }   
}

void EventLoop::handleNewConnection(){
    int peerfd =_acceptor.accept();
    if(peerfd < 0){
        perror("handleNewConnection");
    }
    addEpollReadFd(peerfd);//将新连接请求的fd添加到红黑树中
    //创建TCP连接
    TcpConnectionPtr con(new TcpConnection(peerfd));
    //注册三个事件
    con->setConnectionCallback(_onConnectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    _conns.insert(std::make_pair(peerfd, con));//

    con->handleConnectionCallback();//处理连接请求
}

void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);//查找连接
    if(it != _conns.end())
    {
        bool flag = it->second->isClosed();
        if(flag)//处理连接断开
        {
            it->second->handleCloseCallback();
            delEpollReadFd(fd);//从红黑树中删除
            _conns.erase(it);//从map中删除
        }
        else//处理消息到达的事件
        {
            it->second->handleMessageCallback();
        }
    }
    else{
        cout << "this conns is exist" << endl;
    }
}

//epoll 红黑树(需要被监听的文件描述符)+就绪链表(epoll_wait返回时已经就绪的文件描述符)
int EventLoop::createEpollFd()
{
    int fd = epoll_create1(0);
    if(fd < 0)
    {
        perror("epoll_create1");
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN; //监听读事件

    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD,  fd, &evt);
    if(ret < 0)
    {
        perror("epoll_ctl add");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd){
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN; //监听读事件

    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL,  fd, &evt);
    if(ret < 0)
    {
        perror("epoll_ctl del");
        return;
    }
}
