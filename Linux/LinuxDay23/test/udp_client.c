#include<func.h>

//socket->bind->recvfrom->sendto
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    struct sockaddr_in serAddr;
    memset(&serAddr,0,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);//点分十进制转换成网络字节序
    serAddr.sin_port = htons(atoi(argv[2]));//主机字节序转换成网络字节序
    char buf[64] = {0};
    socklen_t len = sizeof(serAddr);

}
