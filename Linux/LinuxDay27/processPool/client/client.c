#include"function.h"

void sigfunc(int sigNum){
    printf("sig %d is coming\n", sigNum);
    exit(0);
}

int main()
{
    // ./client 
    /****************TCP连接*************/
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketFd, -1, "socket");
    int ret;
    //connect 
    socketInit(socketFd, "127.0.0.1", "8888");   
    printf("connect has completed \n");

    /**************连接数据库************/
    int pid = getpid();
    send(socketFd, &pid, sizeof(int), 0);
    printf("%d\n", pid);
    printf("Please enter your userName:\n");
    //**********登录**********
    char userName[64] = {0};
    scanf("%s",userName);//输入用户名并发送给服务端
    send(socketFd, userName, sizeof(userName), 0);
    //getpass()从控制台输入一行字符串，输入时不显示字符串,密码长度不超过128    
    char *password;//输入密码并发送给服务端
    password = getpass("Please enter your password:\n");
    char salt[64] = {0};
    char cryptpasswd[128] = {0};
    //1.先查询此用户是否注册 ret = 1/0/-1 注册/未注册/出错
    char isRegister[2] = {0};//是否注册标志 1/0 注册/未注册
    char isTruePassword[2] = {0};//密码是否正确标志 1/0 正确/错误
    recv(socketFd, isRegister, sizeof(isRegister), 0);//1.1/1.2接收注册信息
    if(0 == strcmp(isRegister, "0")){//1.1未注册
        //接收服务器随机生成的salt
        puts("No Register");
        recv(socketFd, salt, sizeof(salt), 0);//1.2.1接收salt
        //根据密码和salt生成密文
        strcpy(cryptpasswd, crypt(password, salt));
        //发送密文
        send(socketFd, cryptpasswd, sizeof(cryptpasswd), 0);//1.2.2发送cryptpasswd
        printf("This is your first login and an account has been registered for you\n");
    }else{//1.1已注册
        //1.1.1接收数据库的salt
        recv(socketFd, salt, sizeof(salt), 0);
reEnterPasswd:
        //2.根据salt和密码生成密文
        strcpy(cryptpasswd, crypt(password, salt));
        //1.1.2发送密文
        send(socketFd, cryptpasswd, sizeof(cryptpasswd), 0);
        //1.1.3接收密码是否正确的消息
        recv(socketFd, isTruePassword, sizeof(isTruePassword), 0);
        //2.1密码错误重新输入
        if(0 == strcmp(isTruePassword, "0")){
            password = getpass("Please reenter your password\n");
            goto reEnterPasswd;
        }
        else{//2.2密码正确
            printf("Password is true\n");
        }
    }
    //3.输入命令
    printf("Please enter your command:  \n");
    char cmd[64] = {0};
    int cmdLen = 0;
    cmdMsg_t cmdMsg;
    char currentPath[128] = {0};
    while(1){
        signal(SIGUSR1, sigFunc);//注册10号信号
        memset(currentPath, 0, sizeof(currentPath));
        memset(cmd, 0, sizeof(cmd));
        memset(&cmdMsg, 0, sizeof(cmdMsg));
        scanf("%s",cmd);
        //fgets(cmd, sizeof(cmd), stdin);//先标准输入stdin写入内容
        getCmdType(cmd, &cmdMsg);//解析命令
        printf("%d\n", cmdMsg.cmd);
        for(int i = 0; i < (int)sizeof(cmd) - 1; ++i){
            if(' ' == cmd[i] || '\0' == cmd[i]){
                cmdLen = i + 1;
                break;
            }
        }
        for(int i = 0; i < (int)sizeof(cmd) - 1; ++i){
            if('\n' == cmd[i + cmdLen]){
                break;
            }
            cmdMsg.data[i] = cmd[i + cmdLen];
        }
        cmdMsg.length = strlen(cmdMsg.data);
        send(socketFd, &cmdMsg, sizeof(cmdMsg), 0);//发送命令消息
        switch(cmdMsg.cmd){
            case CD:
                recv(socketFd, currentPath, sizeof(currentPath), 0);
                printf("%s\n", currentPath);
                if('N' == currentPath[0]){
                    printf("Please enter your command:\n");
                }
                lsCmd(currentPath);
                printf("Please enter your command:\n");
                break;
            case LS:
                recv(socketFd, currentPath, sizeof(currentPath), 0);
                lsCmd(currentPath);
                printf("Please enter your command:\n");
                break;
            case PUTS:
                ret = transferFile(socketFd, cmdMsg.data);
                if(-1 == ret){
                    printf("No such file, please reenter your command:\n");
                    break;
                }
                printf("Please enter your command:\n");
                break;
            case GETS:
                ret = recvFile(socketFd);
                //接收noFile发过来的错误消息
                if(-1 == ret){
                    ret = recv(socketFd, currentPath, sizeof(currentPath), 0);
                    printf("%s\n", currentPath);
                }
                printf("Please enter your command:\n");
                break;
            case PWD:
                recv(socketFd, currentPath, sizeof(currentPath), 0);
                printf("%s\n", currentPath);
                printf("Please enter your command:\n");
                break;
            case MKDIR:
                printf("Make dir success\n");
                printf("Please enter your command:\n");
                break;
            case RMDIR:
                printf("Remove dir success\n");
                printf("Please enter your command:\n");
                break;
            case ENTER:
                printf("Please enter your command:\n");
                break;
            case EXIT:
                break;
            default://ERROR
                printf("No such command\n");
                printf("Please reenter your command:\n");
                break;
        }
    }
    close(socketFd);
    return 0;
}

