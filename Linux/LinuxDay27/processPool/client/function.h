#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <func.h>

#define BUFFSIZE 1000

enum command{
    CD = 1,//1.进入服务器目录
    LS,//2.列出服务器上相应的目录和文件
    PUTS,//3.将本地文件上传至服务器
    GETS,//4.下载
    PWD,//5.显示目前所在路径
    MKDIR,//6.创建文件夹
    RMDIR,//7.删除
    ENTER,//8.\n
    EXIT,//9.退出
    ERROR//10.出错
}cmd;
//命令内容 //类型(enum command) 
typedef struct cmdMsg_s{
    int length;
    char data[128];
    enum command cmd; 
}cmdMsg_t;

typedef struct {
	int length;
	char data[BUFFSIZE];
} train_t;

//*******cmdSql.c**********
int getCmdType(char *cmd, cmdMsg_t * cmdMsg);

void printmode(mode_t mode);
int lsCmd(char * currentPath);
//puts 上传 发送文件给服务端
int transferFile(int socketFd, const char * fileName);
//gets 下载 从服务端下载
int recvFile(int socketFd);

int recvn(int socketFd, void *pstart, int len);
//退出
//**********tcp.c*******
void sigFunc(int sigNum);
//客户端tcp连接
int socketInit(int socketFd, char *ip, char * port);

#endif


