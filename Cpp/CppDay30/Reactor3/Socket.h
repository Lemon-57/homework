#pragma once
// 所有与套接字相关操作，全部封装进来
// 套接字是稀缺资源，所以要进行回收，
// 所以可以使用RAII的思想
#include"NonCopyable.h"

class Socket
:NonCopyable
{
public:
    Socket();
    ~Socket();
    explicit Socket(int fd);//只能显示调用，不能隐式调用 禁止使用int fd = 10；
    int fd() const;
    void shutDownWrite();
private:
    int _fd;
};

