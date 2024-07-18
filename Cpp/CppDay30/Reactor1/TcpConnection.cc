#include"TcpConnection.h"
#include<iostream>
# include<sstream>//ostringstream

using std::endl;
using std::cout;
using std::ostringstream;

TcpConnection::TcpConnection(int fd)
:_sock(fd)
,_sockIO(fd)
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
{

}

TcpConnection::~TcpConnection(){
    
}
//发送消息到缓冲区
void TcpConnection::send(const string & msg){
    _sockIO.writen(msg.c_str(), msg.size());
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
//获取本端的网络地址信息
InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);//getsockname 参数三只能传左值
    int ret = getsockname(_sock.fd(), (sockaddr *)&addr, &len);
    if(-1 == ret){
        perror("getsockname");
    }
    return InetAddress(addr);//调用构造函数
}
//获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getpeername(_sock.fd(), (sockaddr *)&addr, &len);
    if(-1 == ret){
        perror("getsockname");
    }
    return InetAddress(addr);
}

