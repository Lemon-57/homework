#ifndef  _WD_TCPHREADER_H_
#define  _WD_TCPHREADER_H_

#define BUFFSIZE 100

#include<func.h>

typedef struct Channel{
    int sockfd;//存储通道数组中的fd属性
    int recvbuffsize;//接收缓冲区已经存放了多少数据
    char recvbuff[BUFFSIZE];//应用层接收缓冲区
    int sendbuffsize;//发送缓冲区还有多少数据
    char sendbuff[BUFFSIZE];//应用层接收缓冲区
}channel_t;

void setNonblock(int fd);

int socketInit();

struct sockaddr_in inetaddress(const char *ip, uint16_t port);

int tcpServerInit(const char *ip,uint16_t port);

int sendn(int fd, const void * buf, int len);

void channelInit(channel_t *p);//初始化一个通道
void channelDestroy(channel_t *p);///针对一个通道的销毁

void channelsInit(channel_t *p,int length);//初始化length长度的通道数组

void clearSendBuff(channel_t *p);//清空发送缓冲区
void clearRecvBuff(channel_t *p);//清空接收缓冲区


void channelAdd(channel_t *p, int length, int fd);//往通道数组中添加通道
void channelDel(channel_t *p, int length, int fd);//删除通道数组中的通道

int channelGetIndex(channel_t *p, int length, int fd);//在通道数组中查找




#endif
