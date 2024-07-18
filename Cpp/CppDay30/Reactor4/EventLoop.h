#pragma once

// 事件循环的类
// 只要accept有正常的返回值，就表明三次握手建立完成了，
// 此时就表明连接已经创建，该连接就是一个Tcp连接，
// 所以可以使用键值对的形式,从语法角度看TcpConnection在此处可以相同；
// 但是在实际情况下面，是具有对象语义的

#include "TcpConnection.h"
#include "MutexLock.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void (const TcpConnectionPtr &)>;
using Functor = function<void()>;

class Acceptor;

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();
    void loop();
    void unloop();
    void runInLoop(Functor &&cb);

    //EvenLoop只需要将三个事件传递给TcpConnection 
    //所以只需要注册，不需要执行， 就不需要对于的handle函数 
    void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

    void doPendingFunctors();
    void handleRead();
    void wakeup();
    int createEventFd();

private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

private:
    int _epfd;
    int _evfd;
    Acceptor &_acceptor;
    bool _isLooping;
    vector<struct epoll_event>_evtList;//存储文件描述符的属性
    //make_pair(peerfd, con) 一个peerfd对应一个新的连接
    map<int, TcpConnectionPtr> _conns;
    
    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

    vector<Functor> _pendingCb;
    MutexLock _mutex;
};

