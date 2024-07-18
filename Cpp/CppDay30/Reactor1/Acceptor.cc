#include"Acceptor.h"

Acceptor::Acceptor(const string &ip, unsigned short port)
:_servAddr(ip, port)
,_listenSock()
{

}

Acceptor::~Acceptor(){

}

void Acceptor::ready(){
    setReuserAddr();
    setReuserPort();
    bind();
    listen();//处于监听状态
}

void Acceptor::setReuserAddr(){
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret){
        perror("setReuserAddr error");
    }
}

void Acceptor::setReuserPort(){
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(-1 == ret){
        perror("setReuserAddr error");
    }
}

void Acceptor::bind(){
    int ret = ::bind(_listenSock.fd(), (struct sockaddr *)_servAddr.getInetAddressPtr(), sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("bind error");
        return;
    }
}

void Acceptor::listen(){
    int ret = ::listen(_listenSock.fd(), 128);
    if(-1 == ret){
        perror("listen error");
        return;
    }
}

//一旦全连接队列中到来新的连接，此时accept读操作就会就绪
int Acceptor::accept(){
    int connfd = ::accept(_listenSock.fd(), nullptr, nullptr);    
    if(-1 == connfd){
        perror("accept error");
        return -1;
    }
    return connfd;
}

int Acceptor::fd()const{
    return _listenSock.fd();
}