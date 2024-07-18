 ///
 /// @file    main.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-01 20:25:01
 ///
 
#include "processpool.h"

int exitpipe[2];

void sigfunc(int num) 
{
	printf("sig %d is coming\n", num);
	//通过管道通知父进程退出
	char ch = 1;
	write(exitpipe[1], &ch, 1);
}
 
int main(int argc, char *argv[])
{//ip,port,processNum
	signal(SIGUSR1, sigfunc);
	ARGS_CHECK(argc, 4);
	int processNum = atoi(argv[3]);
	//子进程的信息记录数组
	pProcess_pool_t pProcesspool = (pProcess_pool_t)
		calloc(processNum, sizeof(process_pool_t));
	//1. 创建N个子进程和N条管道
	makeChild(pProcesspool, processNum);

	pipe(exitpipe);//创建退出的管道

	//2. 创建TCP的监听套接字listenfd
	int listenfd = tcpServerInit(argv[1], atoi(argv[2]));

	//3. 创建epoll的实例, 并注册(监听)相应文件描述符上的事件
	int epfd =  epollCreate();
	epollAddReadEvent(epfd, listenfd);
	for(int i = 0; i < processNum; ++i) {
		epollAddReadEvent(epfd, pProcesspool[i].pipefd);
	}
	epollAddReadEvent(epfd, exitpipe[0]);

	struct epoll_event * pevtList = (struct epoll_event*)
		calloc(MAXCONNS, sizeof(struct epoll_event));
	
	int nready;
	while(1) {
		do {
			//4. epoll等待事件就绪
			nready = epoll_wait(epfd, pevtList, MAXCONNS, 5000);
		}while(-1 == nready && errno == EINTR);

		if(0 == nready) {
			printf(">> epoll timeout.\n");
			continue;
		} else if(-1 == nready) {
			perror("epoll_wait");
			return EXIT_FAILURE;
		} else {//nready > 0
			
			for(int i = 0; i < nready; ++i) {
				int fd = pevtList[i].data.fd;
				if(fd == listenfd) {//5. 客户端到来的处理
					int peerfd = accept(listenfd, NULL, NULL);

					for(int j = 0; j < processNum; ++j) {
						if(0 == pProcesspool[j].busy) {
							//5.1 查找一个空闲的子进程，发送peerfd, 交给子进程进行处理
							sendFd(pProcesspool[j].pipefd, peerfd, PROCESS_NOT_EXIT_FLAG);
							pProcesspool[j].busy = 1;//更改子进程的状态,设置为忙碌的
							break;
						}
					}
				} else if(fd == exitpipe[0]) {
					char exitflag;
					read(exitpipe[0], &exitflag, 1);
					printf("parent process ready to exit.\n");
					//收到退出的通知之后，子进程要逐步退出
					for(int j = 0; j < processNum; ++j) {
						//不需要传递文件描述符，所以第二个参数设置为0
						//第三个参数是子进程退出的标志位
						sendFd(pProcesspool[j].pipefd, 0, PROCESS_READY_EXIT_FLAG);
					}


					//回收子进程的资源, 不回收就会变成僵尸进程
					for(int j = 0; j < processNum; ++j) {
						wait(NULL);
					}
					printf("parent process pool exit\n");
					exit(0);
				} else {
					//6. 子进程任务执行完毕，通知父进程
					//当父进程收到通知(某一个管道有数据过来)时，将子进程标记位空闲的
					int howmany;
					int ret = read(fd,  &howmany, sizeof(howmany));
					printf(">> parent pipe ret:%d\n", ret);
					for(int i = 0; i < processNum; ++i) {
						if(pProcesspool[i].pipefd == fd) {
							pProcesspool[i].busy = 0;//将子进程标记位空闲
							printf("child %d is not busy.\n", pProcesspool[i].pid);
							break;
						}
					}
				}
			}
		}
	}

	free(pevtList); 
	close(listenfd);
	close(epfd);
	return 0;
}
