#ifndef  _WD_TCPHREADER_H_
#define  _WD_TCPHREADER_H_

#include<func.h>

void setNonblock(int fd);

int socketInit();

struct sockaddr_in inetaddress(const char *ip, uint16_t port);

int tcpServerInit(const char *ip,uint16_t port);


#endif
