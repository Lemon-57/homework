#include"worker.h"


int sendn(int sockFd, void * buf, int len){
    int total = len;
    char *pbuf = (char *)buf;
    int ret;
    while(total > 0){
        ret = send(sockFd, pbuf, total, 0);
        if(ret > 0){
            perror("send");
            break;
        }else{    
            total -= ret;
            pbuf += ret;
        }
    }
    return len - total;
}

int transFile(int peerFd){
    
    train_t t = {5, "lemon"};
    send(peerFd, &t, 4 + 5, MSG_NOSIGNAL);
    int fileFd = open(t.buf, O_RDWR); 
    ERROR_CHECK(fileFd, -1, "open");
    
    //获取文件长度
    struct stat st;
    int ret = fstat(fileFd, &st);
    printf("fileSize = %ld\n", st.st_size);
    memset(&t, 0,sizeof(t));
    t.dataLenth = sizeof(st.st_size);
    //发送文件长度 车头4 车厢sizeof(off_t)
    memcpy(t.buf, &st.st_size, t.dataLenth);
    send(peerFd, &t, sizeof(off_t) + 4, MSG_NOSIGNAL);
    //ret = sendn(peerFd, (char *)&t.dataLenth, 4 + sizeof(off_t)); 
    //发送文件内容
    char * pbuf = (char *)mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileFd, 0);
    ERROR_CHECK(pbuf, (void*)-1, "mmap");
    //ret = sendn(peerFd, pbuf, st.st_size);
    send(peerFd, pbuf, st.st_size, MSG_NOSIGNAL);
    printf("file send over!\n");
    munmap(pbuf, st.st_size);
    close(fileFd);
    return 0;
}
