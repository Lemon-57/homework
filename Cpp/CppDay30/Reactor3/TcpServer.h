#pragma once

#include"Acceptor.h"
#include"EventLoop.h"

class TcpServer
{
public:
    TcpServer(const string &ip,  unsigned short port)
    :_acceptor(ip, port)
    ,_loop(_acceptor)
    {

    }
    void start()
    {
        _acceptor.ready();//三次握手完成，此时处于监听状态
        _loop.loop();//启动epoll监听
    }
    
    void setAllCallback(TcpConnectionCallback &&onConnection, 
                        TcpConnectionCallback &&onMesssage, 
                        TcpConnectionCallback &&onClose)
    {   //此处TcpConnectionCallback &&onConnection右值引用有名为左值
        //std::move是将左值转化为右值
        _loop.setConnectionCallback(std::move(onConnection));
        _loop.setMessageCallback(std::move(onMesssage));
        _loop.setCloseCallback(std::move(onClose));
    }
    ~TcpServer() {}

private:
    Acceptor _acceptor;
    EventLoop  _loop;
};

