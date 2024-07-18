#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    puts("pipe1 open");
    int fdw = open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");
    puts("pipe2 open");

    char buf[4096];
    fd_set rdset;
    struct timeval timeout;
    time_t rawtime;
    struct tm *info;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        bzero(&timeout,sizeof(timeout));
        timeout.tv_sec = 60;
        if(FD_ISSET(STDIN_FILENO,&rdset)){  
             memset(buf,0,sizeof(buf));
             read(fdr,buf,sizeof(buf));
             time(&rawtime);
             info = localtime(&rawtime);
             printf("from MM:%3d:%3d:%3d",info->tm_hour,info->tm_min,info->tm_sec);
             printf(":%s\n",buf);
        }
        if(FD_ISSET(fdr,&rdset)){
             memset(buf,0,sizeof(buf));
             int ret = read(STDIN_FILENO,buf,sizeof(buf));
             if(ret == 0)
             {
                 printf("chat is broken");
                 break;
             }
             write(fdw,buf,strlen(buf));
        }
        
    }
    return 0;
}

