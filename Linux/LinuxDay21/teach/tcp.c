 ///
 /// @file    tcp.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-05-28 08:26:36
 ///
 

#include "tcp.h"

void setNonblock(int fd)
{ 
	int flags = fcntl(fd, F_GETFL, 0); 
	flags |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
}

struct sockaddr_in inetaddress(const char * ip, uint16_t port)
{
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	return serveraddr;
}

int socketInit()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0); 
	ERROR_CHECK(listenfd, -1, "socket");
	return listenfd;
}

int tcpServerInit(const char * ip, uint16_t port)
{
	int ret;
	//1. 创建tcp套接字
	int listenfd = socketInit();

	int reuse = 1;
	if(setsockopt(listenfd, SOL_SOCKET, 
			SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) 
	{	perror("setsockopt");	}

	//2. 绑定网络地址
	struct sockaddr_in serveraddr = inetaddress(ip, port);
	ret = bind(listenfd, 
			(struct sockaddr*)&serveraddr, 
			sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "bind");

	//3. 进行监听
	ret = listen(listenfd, 100);
	ERROR_CHECK(ret, -1, "listen");
	printf("listen has completed.\n");
	return listenfd;
}
