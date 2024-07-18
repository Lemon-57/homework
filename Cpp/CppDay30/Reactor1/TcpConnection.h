#pragma once

//该类的创建，表明三次握手已经成立，可以进行数据的收发，也就是业务逻辑的处理
#include"SocketIO.h"
#include"Socket.h"
#include"InetAddress.h"

class TcpConnection
{
public:
    TcpConnection(int fd);
    ~TcpConnection();
    void send(const string & msg);
    string receive();
    string toString();
    InetAddress getLocalAddr();//获取本端的网络地址信息
    InetAddress getPeerAddr();//获取对端的网络地址信息
private:
    SocketIO _sockIO; //数据收发的具体操作
    Socket _sock; //套接字相关操作
    InetAddress _localAddr;//ip端口相关操作
    InetAddress _peerAddr;
};

