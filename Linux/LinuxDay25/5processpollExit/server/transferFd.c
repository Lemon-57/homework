 ///
 /// @file    taransferFd.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-01 11:39:38
 ///
 
#include "transferFd.h"

#include <func.h>

 
int sendFd(int pipefd, int fd, char exitFlag)
{
	//1. 构造iov结构体
	//char buf[10] = "hello";//无效数据
	struct iovec iov;//通过其传递是否退出的标志位信息
	iov.iov_base = &exitFlag;
	iov.iov_len = 1;

	//2. 构造cmsghdr
	int len = CMSG_LEN(sizeof(int));
	struct cmsghdr * p = calloc(1, len);
	p->cmsg_len = len;
	p->cmsg_level = SOL_SOCKET;
	p->cmsg_type = SCM_RIGHTS;
	int * pfd = (int*)(CMSG_DATA(p));
	*pfd = fd;
	//3. 构造msghdr
	struct msghdr msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = p;
	msg.msg_controllen = len;

	int ret = sendmsg(pipefd, &msg, 0);
	ERROR_CHECK(ret, -1, "sendmsg");
}

int recvFd(int pipefd, char * pexitFlag) 
{
	//1. 构造iov结构体
	struct iovec iov;
	iov.iov_base = pexitFlag;
	iov.iov_len = 1;

	//2. 构造cmsghdr
	int len = CMSG_LEN(sizeof(int));
	struct cmsghdr * p = calloc(1, len);
	p->cmsg_len = len;
	p->cmsg_level = SOL_SOCKET;
	p->cmsg_type = SCM_RIGHTS;
	//3. 构造msghdr
	struct msghdr msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = p;
	msg.msg_controllen = len;

	int ret = recvmsg(pipefd, &msg, 0);
	ERROR_CHECK(ret, -1, "recvmsg");
	int fd = *(int*)CMSG_DATA(p);
	return fd;
}

