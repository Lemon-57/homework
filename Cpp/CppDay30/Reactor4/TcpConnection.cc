#include"TcpConnection.h"
#include "EventLoop.h"
#include<iostream>
#include<sstream>//ostringstream

using std::endl;
using std::cout;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
:_loop(loop)
,_sock(fd)
,_sockIO(fd)
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
{

}

TcpConnection::~TcpConnection(){
    
}

//发送消息到缓冲区
void TcpConnection::send(const string & msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

//线程池与IO线程(EventLoop/Reactor)进行通信
void TcpConnection::sendInLoop(const string &msg){
    //消息msg需要从TcpConnection发送到EventLoop
    /* this->send(msg) */
    if(_loop)
    {
        //void runInLoop(function<void()>)
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}

//从缓冲区接收消息
string TcpConnection::receive(){
    char buff[65535] = {0};
    // _sockIO.readn(buff, sizeof(buff)); //readn需要读够65535才能出循环
    _sockIO.readLine(buff, sizeof(buff));
    return string(buff);
}

string TcpConnection::toString(){
    ostringstream oss; //格式化字符串来代替sprintf
    oss << _localAddr.ip() << " : "
        << _localAddr.port() <<" ---> "
        << _peerAddr.ip() << " : "
        << _peerAddr.port();
    return oss.str(); //以字符串的形式输出
}
//从缓冲区读取数据 如果ret = 0表示连接已经断开 
bool TcpConnection::isClosed() const
{
    char buf[10] = {0};
    int ret = recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return ret == 0;
}
//注册回调函数
void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb){
    _onConnectionCb = cb;
}
//注册回调函数
void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb){
    _onMessageCb = cb;
}
//注册回调函数
void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb){
    _onCloseCb = cb;
}

void TcpConnection::handleConnectionCallback()
{
    //this ->TcpConnection > (this - >shared_ptr)
    if(_onConnectionCb)
    {
        /* _onConectionCb(shared_ptr<TcpConnection>(this)); */
        _onConnectionCb(shared_from_this());//分享线程所有权
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb)
    {
        _onCloseCb(shared_from_this());
    }
}


//获取本端的网络地址信息
InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);//getsockname 参数三只能传左值
    int ret = getsockname(_sock.fd(), (sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }
    return InetAddress(addr);//调用构造函数
}
//获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret){
        perror("getsockname");
    }
    return InetAddress(addr);
}

