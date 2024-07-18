#include "function.h"

// puts 上传 用户端发送文件给服务端
int transferFile(int socketFd, const char *fileName)
{
    // 发送文件名
    train_t train;
    memset(&train, 0, sizeof(train));
    train.length = strlen(fileName);
    strcpy(train.data, fileName);
    send(socketFd, &train, sizeof(int) + sizeof(train.data), 0);

    // 获取用户当前的工作路径保存
    char path[128] = {0};
    getcwd(path, sizeof(path));
    // 拼接当前工作路径和文件名
    sprintf(path, "%s", fileName);
    // 读取用户文件
    int fileFd = open(path, O_RDWR);
    ERROR_CHECK(fileFd, -1, "open");
    // 获取文件长度
    struct stat fileInfo;
    fstat(fileFd, &fileInfo);
    printf("fileSize = %ld\n", fileInfo.st_size);
    memset(&train, 0, sizeof(train));
    train.length = sizeof(fileInfo.st_size);
    // 发送文件长度 车头sizeof(int)
    memcpy(train.data, &fileInfo.st_size, train.length);
    send(socketFd, &train, sizeof(int) + train.length, 0);
    // 发送文件内容
    char *pbuf = (char *)mmap(NULL, fileInfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileFd, 0);
    ERROR_CHECK(pbuf, (void *)-1, "mmap");
    send(socketFd, pbuf, fileInfo.st_size, MSG_WAITALL);
    printf("file send over!\n");
    munmap(pbuf, fileInfo.st_size);
    close(fileFd);
    return 0;
}

int recvn(int socketFd, void *pstart, int len)
{
    int total = len;
    int ret = 0;
    char *pbuf = (char *)pstart;
    while (total < len)
    {
        ret = recv(socketFd, pbuf, total, 0);
        if (ret < 0)
        {
            perror("recv");
            break;
        }
        total -= ret;
        pbuf += ret;
    }
    return len - total;
}

// gets 下载 客户端从服务端接收文件
// 分段接收
int recvFile(int socketFd)
{
    char fileName[128] = {0};
    train_t train; // 应用层缓冲区
    memset(&train, 0, sizeof(train));
    // 1.先接收文件名长度和文件名
    recv(socketFd, &train.length, sizeof(int), MSG_WAITALL);
    printf("%d\n", train.length);
    recv(socketFd, train.data, train.length, MSG_WAITALL);
    printf("%s\n", train.data);
    // 获取用户当前的工作路径保存
    char path[128] = {0};
    getcwd(path, sizeof(path));

    // 2.接收服务器里的文件大小
    off_t totalSize;
    train_t trainFileSize;
    memset(&trainFileSize, 0, sizeof(int));
    recv(socketFd, &trainFileSize.length, sizeof(int), MSG_WAITALL);
    recv(socketFd, &totalSize, train.length, MSG_WAITALL);
    printf("fileSize = %ld\n", totalSize);
    // 断点续传
    DIR *pdir = opendir(path);
    ERROR_CHECK(pdir, NULL, "opendir");

    // 打开目录后sleep一秒，让服务端先去准备文件
    sleep(1);

    // 获取当前目录下的文件信息
    struct dirent *pdirent;
    // readdir成功返回当前文件的dirent，并将指针指向下个文件
    off_t downloadSize, remainSize; // fileSize, remainSize已经下载下载的大小，剩余未下载的大小
    strcpy(fileName, train.data);
    // 拼接当前工作路径和文件名
    sprintf(path, "%s", fileName);
    // 接收方(创建/打开)一个同名文件,在当前目录下创建用户文件
    int fileFd = open(path, O_RDWR | O_CREAT, 0666);
    ERROR_CHECK(fileFd, -1, "open");
    while ((pdirent = readdir(pdir)) != NULL)
    {
        // 判断是已经下载一部分的同名文件
        if (0 == strcmp(pdirent->d_name, fileName))
        {
            // 如果存在打开文件
            struct stat fileInfo;
            // 读取fileFd的文件信息到fileInfo中
            fstat(fileFd, &fileInfo);
            // 获取本地已下载文件的大小
            downloadSize = fileInfo.st_size;
            // 通知服务器从哪里开始发
            send(socketFd, &downloadSize, sizeof(off_t), 0);
            // 并将文件指针偏移到末尾开始接收
            lseek(fileFd, downloadSize, SEEK_SET);
            printf("downloadSize = %ld\n", downloadSize);
        }
    }
    // 3.接收文件内容 根据发送方发过来的totalSize传输
    remainSize = totalSize - downloadSize;
    off_t slice = (totalSize) / 100L;
    off_t lastSize = (downloadSize / slice) * slice;
    char buf[1024] = {0};
    while (1)
    {
        int recvSize = recvn(socketFd, buf, remainSize);
        if (0 == recvSize)
        {
            break;
        }
        write(fileFd, buf, recvSize);
        remainSize -= recvSize;   // 剩余下载本次下载量
        downloadSize += recvSize; // 已下载
        if (downloadSize - lastSize >= slice)
        {
            lastSize += slice;
            printf("%ld%%\n", lastSize / slice);
            fflush(stdout);
        }
    }
    printf("100.00%%\n");
    // 解除映射
    close(fileFd);
    closedir(pdir);
    return 0;
}
