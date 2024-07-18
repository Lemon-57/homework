#include <func.h>

char* init_env[] = {"USER=lemon","PATH=/tmp",NULL};

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    pid_t pid = fork();
    ERROR_CHECK(pid, -1, "fork");
    if(pid == 0){
       execle("./add","add",argv[1],argv[2],NULL,init_env); 
    }
    wait(NULL);
    return 0;
}

