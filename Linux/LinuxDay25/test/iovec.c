#include <func.h>

int test0(){
    char buf1[10] = "hello ,";
    char buf2[10] = "world";
    int fd;
    fd = open("file",O_RDWR);
    ERROR_CHECK(fd, -1,"open");
    
    struct iovec vecs[2];//vec（向量） 创建带有两个缓冲区的结构体的数组
    vecs[0].iov_base = buf1;//
    vecs[0].iov_len = strlen(buf1);
    vecs[1].iov_base = buf2;
    vecs[1].iov_len = strlen(buf2);

    //聚集写
    int ret = writev(fd, vecs, 2);
    if(ret > 0){
        perror("writev");
    }
    printf("ret: %d\n",ret);
    close(fd);
}

int test1(){
    char buf1[10] = {0};
    char buf2[10] = {0};
    int fd;
    fd = open("file",O_RDWR);
    ERROR_CHECK(fd, -1,"open");
    
    struct iovec vecs[2];//vec（向量） 创建带有两个缓冲区的结构体的数组
    vecs[0].iov_base = buf1;//
    vecs[0].iov_len = sizeof(buf1);
    vecs[1].iov_base = buf2;
    vecs[1].iov_len = sizeof(buf2);

    //聚集写
    int ret = readv(fd, vecs, 2);
    if(ret > 0){
        perror("writev");
    }
    printf("buf1:%s\n buf2:%s\n",buf1, buf2);
    printf("ret: %d\n",ret);
    close(fd);
}

int main()
{
    //test0();
    test1();
    return 0;
}

