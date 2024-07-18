 ///
 /// @file    server.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-05-30 14:33:06
 ///
 

#include "tcp.h"

#include <func.h>

#define MAXEVNTS 1000

int test0()
{
	int listenfd = tcpServerInit("127.0.0.1", 8888);

	int epfd = epoll_create1(0);
	ERROR_CHECK(epfd, -1, "epoll_create1");

	struct epoll_event * pevtList = (struct epoll_event*)
		calloc(MAXEVNTS, sizeof(struct epoll_event));

	struct epoll_event evt;
	evt.data.fd = listenfd;
	evt.events = EPOLLIN; 
	int ret;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &evt);
	ERROR_CHECK(ret, -1, "epoll_ctl");

	while(1) {
		ret = epoll_wait(epfd, pevtList, MAXEVNTS, -1);
		if(0 == ret) {
			printf(">> epoll_wait timeout.\n");
		} else if(-1 == ret && errno == EINTR) {
			continue;
		} else if (-1 == ret) {
			perror("epoll_wait");
			return EXIT_FAILURE;
		} else {// ret > 0
			
			//遍历struct epoll_event数组就可以了
			for(int idx = 0; idx < ret; ++idx) {
				int fd = pevtList[idx].data.fd;
				if(fd == listenfd) {//新连接到来时的处理
					struct sockaddr_in clientaddr;
					memset(&clientaddr, 0, sizeof(clientaddr));
					socklen_t len = sizeof(clientaddr);
					int peerfd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);
					ERROR_CHECK(peerfd, -1, "accept");

					printf("accept a connection.\n");
					printf("clietip: %s, port: %d\n", 
						inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

					//将peerfd加入到epoll监听队列
					struct epoll_event evt;
					evt.data.fd = peerfd;
					evt.events = EPOLLIN | EPOLLET;
					if(epoll_ctl(epfd, EPOLL_CTL_ADD, peerfd, &evt) < 0) {
						perror("epoll_ctl");
					}
				} else {
					//已经建立好的连接
					if(pevtList[idx].events & EPOLLIN) {//判断是否为读事件
						printf("conn fd %d: read event has happend\n", fd);
#if 0
						char recvbuf[1000] = {0};
						int nret = recv(fd, recvbuf, sizeof(recvbuf), 0);
						if(nret == 0) {
							printf("conn fd %d has closed.\n", fd);
							//要将其从epoll监听队列中删除
							struct epoll_event evt;
							evt.data.fd = fd;
							if(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &evt) < 0) {
								perror("epoll_ctl");
							}
							//关闭连接
							close(fd);
						} else if(-1 == nret && errno == EINTR) {
							continue;
						} else if(-1 == nret) {
							perror("recv");
						} else {//nret > 0
							//执行回显服务
							nret = send(fd, recvbuf, strlen(recvbuf), 0);
							printf("send nret:%d\n", nret);
						}
#endif
					}
				}
			}
		}
	}
	return 0;
}
 
int main(int argc, char *argv[])
{
	test0();
	return 0;
}
