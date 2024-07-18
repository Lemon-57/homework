#pragma once

#include<string>
#include<arpa/inet.h>//struct sockaddr_in

using std::string;

class InetAddress
{
public:
    InetAddress(const string &ip, unsigned short port);
    InetAddress(const struct sockaddr_in &addr);
    string ip() const; //返回ip地址 只能读取类成员数据 不能修改
    unsigned short port() const; //返回端口号
    const struct sockaddr_in * getInetAddressPtr() const; //返回ipv4地址
    ~InetAddress();

private:
    struct sockaddr_in _addr;
};

