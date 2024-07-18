#include"command.h"
#include"threadPool.h"

//解析命令类型
void  writeLog(cmdMsg_t msg, char *username){
    char cmd[64] = {0};
    switch(msg.cmd){
        case CD:
            memcpy(cmd, "cd", sizeof(cmd)); 
            insertLog(username, cmd, msg.data);
            break;
        case LS:
            memcpy(cmd, "ls", sizeof(cmd));
            insertLog(username, cmd, msg.data);
            break;
        case PUTS:
            memcpy(cmd, "puts", sizeof(cmd));
            insertLog(username, cmd, msg.data);
            break;
        case GETS:
            memcpy(cmd, "gets", sizeof(cmd));
            insertLog(username, cmd, msg.data);
            break;
        case PWD:
            memcpy(cmd, "pwd", sizeof(cmd));
            insertLog(username, cmd, msg.data);
            break;
        case MKDIR:
            memcpy(cmd, "mkdir", sizeof(cmd));
            insertLog(username, cmd, msg.data);
            break;
        case RMDIR:
            memcpy(cmd, "rmdir", sizeof(cmd));
            insertLog(username, cmd, msg.data);
            break;
        case ENTER:
            memcpy(cmd, "ENTER", sizeof(cmd));
            insertLog(username, cmd, msg.data);
        case EXIT:
            exit(0);
        default://error
            break;
    }
}
//连接数据库并在数据库中执行命令
//数据库记录日志
//insert into MessageLog(username, cmd, data) values (username, cmd, data);
/*******数据库*******/
int insertLog(char *username, char *cmd, char *data){
    MYSQL *conn = NULL;//已有MYSQL的ip地址
    char host[] = "localhost";//主机名或者ip地址
    char user[] = "root";
    char passwd[] = "123456";
    char database[] = "NetworkDisk";//数据库名称
    //初始化mysql句柄
    conn = mysql_init(NULL);
    //建立连接
    if(mysql_real_connect(conn, host, user, passwd, database, 0, NULL, 0) == NULL){
        printf("error connecting to database: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }
    else{
        printf("connecting to database\n");
    }
    //查询数据
    char query[128] = {0};
    sprintf(query, "insert into MessageLog(username, cmd, data) values ('%s', '%s', '%s')", username, cmd, data);
    printf("%s\n", query);
    //执行以NULL结尾的字符串指定的sql查询语句,成功返回0，错误返回非0
    int ret = mysql_query(conn, query);
    if(ret != 0){
        //返回最近调用MYSQL函数发生的错误信息
        const char *errorStr = mysql_error(conn);
        printf("error query %s\n",errorStr);
        mysql_close(conn);
        return EXIT_FAILURE;
    }else{
        printf("insert into success\n");
    }
    mysql_close(conn);
    return 0;
}

//改变工作目录 
//primePath 当前目录 //data 用户传入路径 //lastPath 记录上一次使用的目录
int cdCmd(char * currentPath,  char * lastPath, char * data)
{   
    int flag = 0;//标记是否存在目录 0/1 存在/不存在
    char newPath[128] = {0};
    if('~' == data[0]){
        memcpy(lastPath, currentPath, sizeof(128));
        memcpy(currentPath, data, sizeof(128));
        return flag;
    }
    else if('/' == data[0]){
        memcpy(lastPath, currentPath, sizeof(128));
        memcpy(currentPath, data, sizeof(128));
        return flag;
    }
    else if('-' == data[0]){
        memcpy(currentPath, lastPath, sizeof(128));
        return flag;
    }
    else{//将用户当前路径与要去的下一级路径拼接
        sprintf(newPath, "%s%c%s", currentPath, '/', data);
        if(NULL == opendir(newPath)){
            printf("No such file or directory\n");
            flag = -1;
            return flag;
        }
        memcpy(lastPath, currentPath, sizeof(128));
        memcpy(currentPath, newPath, sizeof(128));
        return flag;
    }
    return flag;
}

//列出服务器上相应的目录和文件
void lsCmd(int peerFd, const char* primePath){
    send(peerFd, primePath, sizeof(128), 0);
}

//显示目前所在路径
void pwdCmd(int peerFd, char * primePath){
   send(peerFd, primePath, sizeof(128), 0); 
}

////断点续传--wuhongyu
//是否大于100M-wuhongyu
//上传文件
void putsCmd(int peerFd){
    recvFile(peerFd);
}

//下载文件
void getsCmd(int peerFd, char * data){
    int ret = transferFile(peerFd, data);
    char noFile[] = "No such file or directory";
    if(-1 == ret){
        sleep(2); //沉睡2秒 因为客户端也在此时沉睡
        send(peerFd, noFile, sizeof(noFile), 0);
    }
}

//创建文件夹
void mkdirCmd(char * primePath, char *currentPath,  char * data){
    sprintf(currentPath, "%s%c%s", primePath, '/', data);
    mkdir(currentPath, 0777);
    printf("Make directory success\n");
}

//删除文件
void rmCmd(char * primePath, char * currentPath, char * data){
    sprintf(currentPath, "%s%c%s", primePath, '/', data);
    remove(currentPath);
    printf("Remove success\n");
}

//退出函数
