#pragma once


//真正进行数据的收发的类
class SocketIO
{
public:
    explicit SocketIO(int fd);//只能显示调用，不能隐式调用 禁止使用int fd = 10；
    ~SocketIO();
    int readn(char *buf, int len);//传数组和数组大小n，读的数据写入传入的数组中
    int readLine(char *buf, int len);//传数组和数组大小n，读入一行数据写入传入的数组中
    int writen(const char *buf, int len);//传数组和数组大小n，从buf钟读取数据
private:
    int _fd;
};

