#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

ThreadPool * gThreadPool = nullptr;

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    :_msg(msg)
    ,_con(con)
    {

    }
    void process()
    {
        //_msg
        //处理完_msg, 进行相应得编解码、计算
        //处理完成之后, 消息是需要发出去的, 需要一个TCP连接
        //消息得接收与发送此时都是Reactor/EventLoop
        //
        //涉及到计算线程(线程池)与IO线程(EventLoop/Reactor)进行通信
        _con->sendInLoop(_msg);
        //消息要从TcpConnection对象发送给EventLoop进行发送
        //此时Tcpconnection必须要知道EventLoop的存在
        //(在TcpConnection)中添加EventLoop的指针或者引用)
    }
private:
    string _msg;
    TcpConnectionPtr _con;
};

//连接
void onConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has conneted!" << endl;
}
//消息处理
void onMessage(const TcpConnectionPtr &con)
{
    //回显
    string msg = con->receive();
    cout << "recv from client msg : "  << msg << endl;
    //con->send(msg);
    //msg是应该做处理的,就是业务逻辑的处理
    //将msg的处理交给线程池
    MyTask task(msg, con);
    gThreadPool->addTask(std::bind(&MyTask::process, task));
}
//关闭连接
void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!" <<endl;
}

void test(){
    ThreadPool threadPool(4, 10);
    threadPool.start();
    gThreadPool = &threadPool;

    TcpServer server("127.0.0.1", 8888);
    server.setAllCallback(std::move(onConnection),
                          std::move(onMessage),
                          std::move(onClose));

    server.start();
}

int main()
{
    test();
    return 0;
}


