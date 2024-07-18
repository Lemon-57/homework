#include<func.h>
#include"transferFd.h"

//exitFlag为1退出
int sendFd(int pipeFd, int fdToSend, int exitFlag){
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(struct msghdr));
    
    struct iovec iov;
    iov.iov_base = &exitFlag;
    iov.iov_len = sizeof(int);
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
    
    struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1,sizeof(CMSG_LEN(sizeof(int))));
    pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsghdr->cmsg_level = SOL_SOCKET;
    pcmsghdr->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(pcmsghdr) = fdToSend;
    
    hdr.msg_control = pcmsghdr;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    printf("%d\n", exitFlag);
    int ret = sendmsg(pipeFd, &hdr, 0);
    printf("sendmsg\n");
    ERROR_CHECK(ret, -1, "sendmsg");
    return 0;
}

int recvFd(int pipeFd, int *pFd, int *exitFlag){
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(struct msghdr));
    
    struct iovec iov;
    iov.iov_base = exitFlag;
    iov.iov_len = sizeof(int);
    
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;

    int len = CMSG_LEN(sizeof(int));
    struct cmsghdr *pcmsghdr = (struct cmsghdr*)calloc(1,len);
    pcmsghdr->cmsg_len = len;
    pcmsghdr->cmsg_level = SOL_SOCKET;
    pcmsghdr->cmsg_type = SCM_RIGHTS;

    hdr.msg_control = pcmsghdr;
    hdr.msg_controllen = len;
    
    int ret = recvmsg(pipeFd, &hdr, 0);
    ERROR_CHECK(ret, -1, "recvmsg");
    *pFd = *(int *)CMSG_DATA(pcmsghdr);
    printf("%d\n", *exitFlag);
    return 0;
}
