#pragma once

#include<hiredis/hiredis.h>
#include<iostream>
#include<string>

using std::cout;
using std::endl;

using std::string;

class MyRedis
{
public:
    MyRedis();
    ~MyRedis();
    bool connect(const string &host, int port);
    void set(string key, string value);
    string get(string key);
private:
    redisContext * _pConnect;//连接redis数据库的返回值 记录连接信息的结构体 
    redisReply * _pReply;//发送请求命令
};

