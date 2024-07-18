#include <func.h>

int main()
{
    int shmid = shmget(1000,4096,0666|IPC_CREAT);
    ERROR_CHECK(shmid,-1,"shmget");
    char *p = (char*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char*)-1,"shmget");
    printf("shmid ID %d\n",shmid);
    puts(p);
    shmdt(p);
    return 0;
}

