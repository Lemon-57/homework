#include "MyRedis.h"

MyRedis::MyRedis()
:_pConnect(nullptr)
,_pReply(nullptr)
{
    cout << "Myredis()" <<endl;
}

MyRedis::~MyRedis(){
    cout << "~MyRedis()"<<endl;
    if(_pConnect){//释放redisConnect()所产生的连接
        redisFree(_pConnect);
        _pConnect = nullptr;
    }
    if(_pReply){//释放redisCommand执行后返回的的redisReply所占用的内存
        freeReplyObject(_pReply);
        _pReply = nullptr;
    }
}

bool MyRedis::connect(const string &host, int port){
    _pConnect = redisConnect(host.c_str(), port);
    if(_pConnect == nullptr){
        return false; // 如果为空直接返回
    }
    if(_pConnect != nullptr && _pConnect->err){//不为空但是标志不为0 正确标志为0
        std::cerr << "connect error ： " << _pConnect->errstr << endl; 
        return false;
    }
    return true;
}

void MyRedis::set(string key, string value){
   //发送请求命令 第一个参数为连接数据库返回的值，剩余的是可变参数
    _pReply = (redisReply *)redisCommand(_pConnect, "SET %s %s", key.c_str(), value.c_str());
    freeReplyObject(_pReply);
    _pReply = nullptr;
}

string MyRedis::get(string key){
    _pReply = (redisReply *)redisCommand(_pConnect, "Get %s", key.c_str());
    string str = _pReply->str;

    freeReplyObject(_pReply);
    _pReply = nullptr;
    return str;
}

