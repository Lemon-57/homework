#include <func.h>

//车厢结构体
typedef struct train_s{
    int dataLength;
    char buf[1024];
}train_t;

//循环接收
int recvn(int clientFd, void *buf, int len);
//分段接收
int recvFile(int clientFd);

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);   
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockFd, -1, "socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);//网络地址字符串转转网络字节序
    addr.sin_port = htons(atoi(argv[2]));//主机字节序转网络字节序

    int ret = connect(sockFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    ERROR_CHECK(ret, -1, "connect");
    printf("connect has completed \n");

    recvFile(sockFd);
    close(sockFd);
    return 0;
}

int recvn(int clientFd, void * buf, int len){
    int total = 0;//已经接收的总长度
    int ret;//本次接收的长度
    char *p = (char*)buf;//指向要接收buf的起始地址
    //接收的总长度小于传入长度一之接收
    while(total < len){
        ret = recv(clientFd, p + total, len - total, 0 );//从已经接收过的地址后后面开始接收
        total += ret; //每次接收的字节数加到total上
    }
}

//分段接收 两段火车 文件名长度——文件名，文件长度——文件大小
//文件内容——mmap
int recvFile(int clientFd){
    train_t t;//应用层缓冲区
    memset(&t, 0, sizeof(t));
    //先接收文件名长度
    recvn(clientFd, &t.dataLength, sizeof(int));
    //再接收文件名
    printf("%d\n",t.dataLength);
    recvn(clientFd, t.buf, t.dataLength);
    //接收方创建一个同名文件
    printf("%s\n",t.buf);
    int fileFd = open(t.buf, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fileFd, -1, "open");
    //读取文件内容并写入到同名文件中//接收第二段火车
    //接收文件大小
    off_t fileSize;
    memset(&t, 0,sizeof(t));
    //接收文件长度
    recvn(clientFd, &t.dataLength, sizeof(int));
    //接收文件大小
    recvn(clientFd, &fileSize, t.dataLength);
    printf("fileSize = %ld\n", fileSize);
    
    ftruncate(fileFd, fileSize);
    //映射
    char *p = (char *)mmap(NULL, fileSize, PROT_READ|PROT_WRITE, MAP_SHARED, fileFd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap");
    recvn(clientFd, p, fileSize);
    printf("100.00%%\n");
    //解除映射
    munmap(p, fileSize);
    close(fileFd);
}
