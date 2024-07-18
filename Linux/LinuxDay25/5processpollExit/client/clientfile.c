 ///
 /// @file    client.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-05-27 16:38:19
 ///
 
#include <func.h>
 
int main(int argc, char *argv[])
{
	//1. 创建套接字
	int clientfd =  socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(clientfd, -1, "socket");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8888);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//2. 向服务器发起建立连接的请求
	int ret = connect(clientfd, 
			(struct sockaddr *)&serveraddr, 
			sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "connect");
	printf("connect has completed.\n");

	//3. 下载文件
	//3.1 接收文件的名字
	int length;
	ret = recv(clientfd, &length, 4, MSG_WAITALL);
	printf(">>1. filename length:%d\n", length);
	
	//提出新的需求: 需要接收确定字节数的数据
	char recvBuff[1000] = {0};//应用层接收缓冲区
	ret = recv(clientfd, recvBuff, length, MSG_WAITALL);
	printf(">>2. recv filename:%s\n", recvBuff);

	//3.2 在客户端本地，要创建文件
	int fileFd = open(recvBuff, O_CREAT|O_RDWR, 0644);

	//3.3 接收文件的大小
	off_t fileSize;
	ret = recv(clientfd, &length, 4, MSG_WAITALL);
	printf("length: %d\n", length);
	ret = recv(clientfd, &fileSize, sizeof(fileSize), MSG_WAITALL);
	printf("> file length: %ld\n", fileSize);

	ftruncate(fileFd, fileSize);

	//3.4 接收文件的内容
	char * pbuf = (char*) mmap(NULL, fileSize, 
			PROT_READ|PROT_WRITE, MAP_SHARED, fileFd, 0);
	ret = recv(clientfd, pbuf, fileSize, MSG_WAITALL);

	close(clientfd);
	close(fileFd);
	munmap(pbuf, fileSize);
}
