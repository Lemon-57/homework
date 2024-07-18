#include "Socket.h"
#include<sys/socket.h>//socket
#include<stdio.h>
//#include<sys/types.h>
#include<unistd.h> //close

Socket::Socket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);//IPv4 地址 TCP协议 协议编号默认0
    if(_fd < 0){
        perror("socket");
        return;
    }
}

Socket::~Socket()
{
    close(_fd);
}

//只能显示调用，不能隐式调用 禁止使用int fd = 10；
Socket::Socket(int fd)
:_fd(fd)
{

}

int Socket::fd() const{//提供给其它组合获取fd
    return _fd;
}

//关闭写端
void Socket::shutDownWrite(){
    int ret = shutdown(_fd, SHUT_WR);//关闭连接，而不是套接字 //SHUT_WR断开输出流。套接字无法发送数据
    if(ret){
        perror("shutDownWrite");
        return;
    }
}
