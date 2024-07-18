 ///
 /// @file    tcp.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-05-28 08:24:11
 ///
 
#ifndef __WD_TCPHEADER_H__
#define __WD_TCPHEADER_H__

#include <func.h>

void setNonblock(int fd);

int socketInit();

struct sockaddr_in inetaddress(const char * ip, uint16_t port);

int tcpServerInit(const char * ip, uint16_t port);


 
 
 
#endif
