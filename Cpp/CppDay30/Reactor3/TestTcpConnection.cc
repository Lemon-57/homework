#include"TcpServer.h"
#include <iostream>
#include<unistd.h>

using std::cout;
using std::endl;

void onConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has conneted!" << endl;
}

void onMessage(const TcpConnectionPtr &con)
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


