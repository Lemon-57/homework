#pragma once

//该类的创建，表明三次握手已经成立，可以进行数据的收发，也就是业务逻辑的处理
#include"SocketIO.h"
#include"Socket.h"
#include"InetAddress.h"

#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;

class TcpConnection
:public std::enable_shared_from_this<TcpConnection> //使用shared_from_this() 要继承该类
{
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void (const TcpConnectionPtr &)>;

public:
    TcpConnection(int fd);
    ~TcpConnection();
    void send(const string & msg);
    string receive();
    string toString();

    bool isClosed() const;
    //注册三个回调函数
    void setConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();//获取本端的网络地址信息
    InetAddress getPeerAddr();//获取对端的网络地址信息
private:
    SocketIO _sockIO; //数据收发的具体操作
    Socket _sock; //套接字相关操作
    InetAddress _localAddr;//ip端口相关操作
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnectionCb;//声明 回调函数
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

