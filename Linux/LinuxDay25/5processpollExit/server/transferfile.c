 ///
 /// @file    transferfile.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-02 11:22:31
 ///
 
#include "processpool.h"
#include <sys/sendfile.h>

int transferFile(int peerfd, const char * filename)
{
	train_t train;
	memset(&train, 0, sizeof(train));
	//3.1 先发文件名给对端
	train.length = strlen(filename);
	strncpy(train.data, filename, train.length);
	int ret = sendn(peerfd, &train, 4 + train.length);

	//3.2 先读取server本地的文件
	char sendBuf[SENDBUFFSIZE] = {0};
	int fileFd = open(filename, O_RDWR);

	//3.3 发送文件的长度
	struct stat st;
	fstat(fileFd, &st);
	printf(">> file length:%ld\n", st.st_size);
	memset(&train, 0, sizeof(train));
	train.length = sizeof(off_t);
	strncpy(train.data, (char*)&st.st_size, sizeof(off_t));
	ret = sendn(peerfd, &train, 4 + train.length);

	//3.4 发送文件内容
	ret = sendfile(peerfd, fileFd, NULL, st.st_size);
	printf("sendfile send over!\n");
	close(fileFd);
	return 0;
}
