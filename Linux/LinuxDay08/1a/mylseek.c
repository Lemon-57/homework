#include <func.h>

typedef struct Student {
    int number;
    char name[20];
    int score;
}Stu;

int main(int argc, char *argv[])
{
    Stu s[3] = {{1,"li",100},{2,"lq",88},{3,"la",90}};
    Stu s1[3] = {0};
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    for(int i = 0; i < 3; i++){ 
         write(fd, &s[i],sizeof(Stu));
    }
    lseek(fd,0,SEEK_SET);
    int ret = read(fd, &s1,sizeof(Stu)*3);
    for(int i = 0; i < 3; i++){ 
         printf("%d, %s, %d\n",s1[i].number,s1[i].name,s1[i].score);
    }
    close(fd);
    return 0;
}

