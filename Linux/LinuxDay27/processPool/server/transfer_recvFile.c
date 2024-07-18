#include"threadPool.h"

//gets 下载 服务器将文件发送给用户
int transferFile(int peerFd, char * fileName){
    //1.一起发送文件名和文件名长度
    train_t train;
    memset(&train, 0, sizeof(train));
    train.length = strlen(fileName);
    strcpy(train.data, fileName);
    send(peerFd, &train, sizeof(int) + sizeof(train.data), 0);
    
    //读取本地文件
    int fileFd = open(fileName, O_RDWR); 
    ERROR_CHECK(fileFd, -1, "open");
    //获取文件长度
    struct stat fileInfo;
    //读取fileFd的文件信息到fileInfo中
    fstat(fileFd, &fileInfo);
    printf("fileSize = %ld\n", fileInfo.st_size);
    memset(&train, 0,sizeof(train));
    train.length = sizeof(fileInfo.st_size);
    //2.发送文件长度  车头sizeof(int)
    memcpy(train.data, &fileInfo.st_size, train.length);
    send(peerFd, &train, sizeof(int) + train.length, 0);
    //从客户端接收过来已经下载的文件的大小
    off_t downloadSize = 0;
    //得到偏移信息
    recv(peerFd, &downloadSize, sizeof(off_t), 0); 
    //先偏移文件指针再进行文件传输
    lseek(fileFd, downloadSize, SEEK_SET);
	printf("downloadSize = %ld\n", downloadSize);
    //3.发送文件内容
    char * pbuf = (char *)mmap(NULL, fileInfo.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileFd, 0);
    ERROR_CHECK(pbuf, (void*)-1, "mmap");
    send(peerFd, pbuf, fileInfo.st_size, MSG_WAITALL);
    printf("file send over!\n");
    munmap(pbuf, fileInfo.st_size);
    close(fileFd);
    return 0;
}

//puts 服务器接收客户端上传文件
//分段接收 两段火车 文件名长度——文件名，文件长度——文件大小
//文件内容——mmap
int recvFile(int socketFd){
    train_t train;//应用层缓冲区
    memset(&train, 0, sizeof(train));
    //先接收文件名长度
    recv(socketFd, &train.length, sizeof(int), 0);
    //再接收文件名
    printf("%d\n",train.length);
    recv(socketFd, train.data, train.length, 0);
    //接收方创建一个同名文件
    printf("%s\n",train.data);
    int fileFd = open(train.data, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fileFd, -1, "open");
    //读取文件内容并写入到同名文件中//接收第二段火车
    //接收文件大小
    off_t fileSize;
    memset(&train, 0, sizeof(train));
    //接收文件长度
    recv(socketFd, &train.length, sizeof(int), 0);
    //接收文件大小
    recv(socketFd, &fileSize, train.length, 0);
    printf("fileSize = %ld\n", fileSize);
    
    ftruncate(fileFd, fileSize);
    //映射
    char *pbuf = (char *)mmap(NULL, fileSize, PROT_READ|PROT_WRITE, MAP_SHARED, fileFd, 0);
    ERROR_CHECK(pbuf, MAP_FAILED, "mmap");
    recv(socketFd, pbuf, fileSize, MSG_WAITALL);
    printf("100.00%%\n");
    //解除映射
    munmap(pbuf, fileSize);
    close(fileFd);
    return 0;
}
