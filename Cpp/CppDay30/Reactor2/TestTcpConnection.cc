#include"TcpConnection.h"
#include"Acceptor.h"
#include"EventLoop.h"
#include <iostream>
#include<unistd.h>

using std::cout;
using std::endl;

void onConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has conneted!" << endl;
}

void onMesssage(const TcpConnectionPtr &con)
{
    //回显
    string msg = con->receive();
    cout << "recv from client msg : "  << msg << endl;
    con->send(msg);
}

void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!" <<endl;
}

void test(){
    Acceptor acceptor("127.0.0.1", 8888);  
    acceptor.ready();//此时处于监听状态

    EventLoop loop(acceptor);
    //注册三个事件
    loop.setConnectionCallback(std::move(onConnection));
    loop.setMessageCallback(std::move(onMesssage));
    loop.setCloseCallback(std::move(onClose));
    //启动
    loop.loop();
}

int main()
{
    test();
    return 0;
}


