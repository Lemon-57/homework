#include"SocketIO.h"
#include<unistd.h>
#include<sys/socket.h>
#include<stdio.h>
#include<errno.h> //errno

//只能显示调用，不能隐式调用 禁止使用int fd = 10；
SocketIO::SocketIO(int fd)
:_fd(fd)
{

}

SocketIO::~SocketIO(){
    close(_fd);
}

//循环读取
int SocketIO::readn(char *buf, int len){//传数组和数组大小n，读的数据写入传入的数组中
    int left = len;
    char * pstr = buf;//成功返回读取的字节数，返回0表示在文件末尾 出现错误时，返回-1，并适当地设置errno。
    int ret = 0;
    while (left > 0)
    {
        ret = read(_fd, pstr, len);//文件描述符 缓冲区 长度
        if(-1 == ret && errno == EINTR){//EINTR 表示中断错误
            continue;
        }
        else if(-1 == ret)
        {   
            perror("readn error");
            return len - ret;
        }
        else if(0 == ret)//已经读到文件的末尾
        {    
            break;
        }
        else{
            left -= ret;
            pstr += ret;
        }
    }
    return len - left;//已经读取到字节数
}

//传数组和数组大小n，读入一行数据写入传入的数组中
int SocketIO::readLine(char *buf, int len){
    int left = len - 1;
    int total = 0;
    char *pstr = buf;
    int ret = 0;
    while (left > 0)
    {
        //MSG_PEEK不会移走缓冲区中数据，只会做拷贝操作
        ret = recv(_fd, pstr, left, MSG_PEEK);
        if(-1 == ret && errno == EINTR){
            continue;
        }
        else if(-1 == ret){
            perror("readLine errno");
        }
        else if(0 == ret){
            break;
        }else{
            for(int idx = 0; idx < ret; ++idx){
                if('\n' == pstr[idx]){
                    int sz = idx + 1;
                    readn(pstr, sz);
                    pstr += sz;
                    *pstr = '\0'; 
                    
                    return total + sz;
                }
            }
            //如果 没有读到'\n'
            readn(pstr, ret);
            pstr += ret;
            total += ret;
            left -= ret;
        }
    }
    *pstr = '\0';
    return total - left;//left出循环时已经等于0 返回读取的总字节
}

int SocketIO::writen(const char *buf, int len){
    int left = len;
    const char * pstr = buf;//成功返回读取的字节数，返回0表示在文件末尾 出现错误时，返回-1，并适当地设置errno。
    int ret = 0;
    while (left > 0)
    {
        ret = write(_fd, pstr, len);//文件描述符 缓冲区 长度
        if(ret == -1 && errno == EINTR){//EINTR 表示中断错误
            continue;
        }
        else if(-1 == ret)
        {   
            perror("writen error");
            return len - ret;
        }
        else if(0 == ret){//已经读到文件的末尾
            break;
        }
        else{
            left -= ret;
            pstr += ret;
        }
    }
    return len - left; //已经读取到字节数
}
