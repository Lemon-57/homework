#pragma once

//将所有的函数，包括bind，listen，端口复用，accept包含在该类中

#include"InetAddress.h"
#include"Socket.h"

class Acceptor
{
public:
    Acceptor(const string &ip, unsigned short port);
    ~Acceptor();
    void ready();
    void setReuserAddr();
    void setReuserPort();
    void bind();
    void listen();
    int accept();
    int fd()const;
private:
    Socket _listenSock;
    InetAddress _servAddr;
};

