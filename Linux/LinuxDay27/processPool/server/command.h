#ifndef _COMMAND_H_
#define _COMMAND_H_

enum command{
    CD = 1,//1.进入服务器目录
    LS,//2.列出服务器上相应的目录和文件
    PUTS,//3.将本地文件上传至服务器
    GETS,//4.下载
    PWD,//5.显示目前所在路径
    MKDIR,//6.创建文件夹
    RMDIR,//7.删除
    ENTER,//8.\n
    EXIT,//9.退出
    ERROR//10.出错
}cmd;
//命令内容 //类型(enum command) 
typedef struct cmdMsg_s{
    int length;
    char data[128];
    enum command cmd; 
}cmdMsg_t; 

//cmdSql.c
//拷贝命令记录
void  writeLog(cmdMsg_t msg, char *username);
//连接数据库并在数据库中执行命令
//解析完插入数据库日志messageLog
//*******数据库*******//数据库 用户命令表 SqlCmd
int insertLog(char *username, char *cmd, char *data);
//********命令*********
//改变工作目录
int  cdCmd(char * , char * , char *);
//列出服务器上相应的目录和文件
void lsCmd(int , const char * );
//显示目前所在路径
void pwdCmd(int , char *);
////断点续传--wuhongyu
//是否大于100M-wuhongyu
//上传文件
void putsCmd(int );
//下载文件
void getsCmd(int , char * );
//创建文件夹
void mkdirCmd(char * , char * , char *);
//删除文件
void rmCmd(char * , char * , char *);
//退出函数
void exit();
//login.c
/********* 登录*******/ //数据库用户信息表 UserInfo
//生成随机字符串
int generateSalt(char *salt);
//注册 插入数据(用户名 盐值 密文)
int insertUserInfo(char *username, char *salt, char *cryptpasswd);
//查询 用户是否存在 存在取出salt
int queryUserInfo(char *username, char *salt);
//查询 密码是否正确
int querrycryptpasswd(char *cryptpasswd);
//登录
int userLogin(int peerFd, char * username);


#endif


