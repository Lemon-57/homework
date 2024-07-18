#include <func.h>
#include"transferFd.h"

int sendFd(int pipefd, int fd){//(管道的文件描述符，传递的文件描述符)
    //1.构造iov结构体，存储需要传递的内容
    char buf[10] = "111:";
    struct iovec iov;//msghdr结构体的成员
    iov.iov_base = buf;//起始地址
    iov.iov_len =strlen(buf);//长度

    //2.构造cmsghdr 附属数据的填充（将文件描述符的信息存入msg_control中，
    //      存储地址值，该地址指向了一个struct_cmsghdr类型的控制信息中）
    int len = CMSG_LEN(sizeof(int));//宏函数计算附属数据的字节数
    struct cmsghdr * pcmsghdr = (struct cmsghdr *)calloc(1,len);
    pcmsghdr->cmsg_len = len;
    pcmsghdr->cmsg_level = SOL_SOCKET;//如果想设置套接字的属性设置为SOL_SOCKET,
    //                                    协议级别
    pcmsghdr->cmsg_type = SCM_RIGHTS;//表示在socket层传输的是文件描述符
    //int * pfd = (int*)(CMSG_DATA(pcmsghdr));//返回指向cmsghdr的数据部分的指针
    //*pfd = fd;//数据部分是文件描述符，将fd放入
    *(int *)(CMSG_DATA(pcmsghdr)) = fd;
    //3.构造msghdr（sendmsg的参数)
    struct msghdr msg;//消息头部
    memset(&msg,0,sizeof(msg) );
    msg.msg_iov = &iov;//1.将传递的内容存储进msg_iov中
    msg.msg_iovlen = 1;//数组元素的个数
    msg.msg_control = pcmsghdr;//2.控制信息的填充
    msg.msg_controllen = len;//控制信息的总字节数

    int ret = sendmsg(pipefd, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    return 0;
}

int recvFd(int pipefd, int *pFd)//sendFd中的pcmsghdr的DATA返回的是一个指针
{
    //1.构造iov结构体
    char buf[10] = {0};
    struct iovec iov;//msghdr结构体的成员
    iov.iov_base = buf;//起始地址
    iov.iov_len =sizeof(buf);//长度

    //2.构造cmsghdr 附属数据的填充（将文件描述符的信息存入msg_control中，
    //      存储地址值，该地址指向了一个struct_cmsghdr类型的控制信息中）
    int len = CMSG_LEN(sizeof(int));//宏函数计算附属数据的字节数
    struct cmsghdr * pcmsghdr = (struct cmsghdr *)calloc(1,len);
    pcmsghdr->cmsg_len = len;
    pcmsghdr->cmsg_level = SOL_SOCKET;//如果想设置套接字的属性设置为SOL_SOCKET,
    //                                    协议级别
    pcmsghdr->cmsg_type = SCM_RIGHTS;//表示在socket层传输的是访问权力 
    
    //3.构造msghdr（sendmsg的参数)
    struct msghdr msg;//消息头部
    memset(&msg,0,sizeof(msg));
    //msg的第二组参数为IO向量缓冲区无需填充
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = pcmsghdr;//2.控制信息的填充
    msg.msg_controllen = len;//控制信息的总字节数
    
    int ret = recvmsg(pipefd,&msg, 0);
    ERROR_CHECK(ret, -1,"recvmsg");
    printf("%s\n",buf);
    *pFd = *(int *)CMSG_DATA(pcmsghdr);
}

