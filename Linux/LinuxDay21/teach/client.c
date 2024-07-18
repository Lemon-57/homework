///
/// @file    client.c
/// @author  lemon(haohb13@gmail.com)
/// @date    2022-05-27 16:38:19
///

#include <func.h>

int main(int argc, char *argv[])
{
	// 1. 创建套接字
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(clientfd, -1, "socket");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8888);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 2. 向服务器发起建立连接的请求
	int ret = connect(clientfd,
					  (struct sockaddr *)&serveraddr,
					  sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "connect");
	printf("connect has completed.\n");

	fd_set rdset;

	while (1)
	{
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO, &rdset);
		FD_SET(clientfd, &rdset);
		char buff[1000] = {0}; //应用层发送和接收缓冲区
		select(10, &rdset, NULL, NULL, NULL);
		if (FD_ISSET(STDIN_FILENO, &rdset))
		{
			ret = read(STDIN_FILENO, buff, sizeof(buff));
			ret = send(clientfd, buff, strlen(buff) - 1, 0);
			memset(buff, 0, sizeof(buff));
		}
		if (FD_ISSET(clientfd, &rdset))
		{
			ret = recv(clientfd, buff, sizeof(buff), 0);
			printf("recv from server:%s\n", buff);
			if (strcmp(buff, "bye") == 0)
			{
				FD_CLR(clientfd, &rdset);
				close(clientfd);
				break;
			}
		}
	}

	close(clientfd);
}
