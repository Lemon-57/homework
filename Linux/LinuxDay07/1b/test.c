#include <func.h>

void printmode(mode_t mode);

int main(int argc,char* argv[]){
    ARGS_CHECK(argc,2);
    DIR* pdir=opendir(argv[1]);
    ERROR_CHECK(pdir,NULL,"opendir");

    chdir(argv[1]);

    struct dirent* pdirent;
    //获取目录项信息
    while((pdirent=readdir(pdir))!=NULL){
        struct stat statbuf;
        //获取时间信息
        struct tm *tmbuf;
        //stat获得文件信息
        int retVal=stat(pdirent->d_name,&statbuf);
        ERROR_CHECK(retVal,-1,"stat");
        tmbuf=localtime(&statbuf.st_mtime);
        ERROR_CHECK(tmbuf,NULL,"localtime");
        //打印文件类型及权限
        printmode(statbuf.st_mode);
        //打印其他信息
        printf(" %3ld %8s %8s %8ld %2d月  %2d %2d:%2d  %-s\n",
                                            statbuf.st_nlink,
                                            getpwuid(statbuf.st_uid)->pw_name,
                                            getgrgid(statbuf.st_gid)->gr_name,
                                            statbuf.st_size,
                                            tmbuf->tm_mon,
                                            tmbuf->tm_mday,
                                            tmbuf->tm_hour,
                                            tmbuf->tm_min,
                                            pdirent->d_name);
    }
    closedir(pdir);
    return 0;
}

void printmode(mode_t mode){
     switch (mode & S_IFMT) {
        case S_IFBLK:  printf("b");     break;
        case S_IFCHR:  printf("c");     break;
        case S_IFDIR:  printf("d");     break;
        case S_IFIFO:  printf("p");     break;
        case S_IFLNK:  printf("l");     break;
        case S_IFREG:  printf("-");     break;
        case S_IFSOCK: printf("s");     break;
        default:       printf("unknown");  break;
    }
    int ugo[3]={0};
    for(int i=0;i<3;i++){
        ugo[2-i]=mode&007;
        mode>>=3;
    }
    for(int i=0;i<3;i++){
        switch(ugo[i]){
        case 1: printf("--x");    break;
        case 2: printf("-w-");    break;
        case 3: printf("-wx");    break;
        case 4: printf("r--");    break;
        case 5: printf("r-x");    break;
        case 6: printf("rw-");    break;
        case 7: printf("rwx");    break;
        default:printf("unknown");    break;
        }
    }
}
