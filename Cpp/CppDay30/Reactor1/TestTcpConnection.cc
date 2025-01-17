#include"TcpConnection.h"
#include"Acceptor.h"
#include <iostream>
#include<unistd.h>

using std::cout;
using std::endl;

void test(){
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();//此时处于监听状态

    //三次握手建立，可以创建一条tcp连接
    TcpConnection con(acceptor.accept());
    cout << con.toString() << " has connected " <<endl;
    while (1)
    {
        cout << ">>recv msg from client " << con.receive() << endl;
        con.send("hello baby\n");
    }
    
}

int main()
{
    test();
    return 0;
}


