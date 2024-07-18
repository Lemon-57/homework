#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR|O_CREAT, 0777);
    ERROR_CHECK(fd, -1,"open");
    struct stat statbuf;
    fstat(fd, &statbuf);
    off_t offset = 0;

    char *pstr = (char *)mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE|PROT_EXEC, MAP_SHARED, fd, offset);   
   
    char *pstr1 = "wworld";
    memcpy(pstr,pstr1,statbuf.st_size);
    
    munmap(pstr, statbuf.st_size);
    return 0;
}

