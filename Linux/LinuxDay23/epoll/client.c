#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd, -1,"socket");
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_port = htons(atoi(argv[2]));
    int ret = connect(sfd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret, -1,"accept");
    char buf[64] = {0};
    fd_set rdset;
    time_t rawtime;
    struct tm *info;
    signal(SIGPIPE,SIG_IGN);
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        select(sfd+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(buf,0,sizeof(buf));
            int ret = read(STDIN_FILENO,buf,sizeof(buf));
            ERROR_CHECK(ret,-1,"read");
            if(ret == 0){
                break;
            }
            send(sfd,buf,strlen(buf)-1,0);//strlen-1 减去read读进来的换行符
        }
        else if(FD_ISSET(sfd,&rdset)){
            memset(buf,0,sizeof(buf));
            ret = recv(sfd,buf,sizeof(buf),0);
        
            if(0 == ret) //ret = 0不会报错，把0写在前面少写 = 号会报错
            {
                printf("byebye\n");
                close(sfd);
                return 0;
            }
            time(&rawtime);
            info = localtime(&rawtime);
            printf("%2d:%2d:%2d from:   %s\n",info->tm_hour,info->tm_min,info->tm_sec ,buf);
        }
    }
    close(sfd);
    return 0;
}

