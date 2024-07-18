 ///
 /// @file    child.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-01 20:28:47
 ///
 
#include "processpool.h"
 
void makeChild(pProcess_pool_t pool, int processNum)
{
	for(int i = 0; i < processNum; ++i) {
		int pipefds[2];
		socketpair(AF_LOCAL, SOCK_STREAM, 0, pipefds);
		pid_t pid = fork();
		if(0 == pid) {//子进程要做的是任务
			close(pipefds[0]);//子进程是要做发送操作,通知父进程任务结束的
			childHandleTask(pipefds[1]);//一直在循环
			exit(0);
		}
		//父进程记录子进程的信息
		close(pipefds[1]);
		pool[i].pid = pid;
		pool[i].pipefd = pipefds[0];
	}
}

void childHandleTask(int pipefd)
{
	printf("child process %d is running\n", getpid());
	while(1) {
		//1. 当没有任务到来时，子进程阻塞在pipefd
		//
		//2. 当recvFd返回时， 表示拿到了任务
		//peerfd没有被epoll监听
		char exitFlag;
		int peerfd = recvFd(pipefd, &exitFlag);//阻塞式函数
		if(0 == exitFlag) {
			//3. 发送文件
			char filename[100] = "bigfile.avi";
			transferFile(peerfd, filename);
			close(peerfd);//关闭与客户端的连接
			//4. 通知父进程, 任务已执行结束
			int one = 1;
			int ret = write(pipefd, &one, sizeof(one));
			printf("child notify parent ret: %d\n", ret);
		} else {
			printf("child %d is exit\n", getpid());
			break;
		}
	}
}

