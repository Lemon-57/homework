#include "command.h"
#include<func.h>
#define STR_LEN 10//定义随机输出的字符串长度
//salt是一个随机数
int generateSalt(char *salt){
    //char salt[STR_LEN + 1] = {0};//传入salt的长度大于10
    int flag;
    srand(time(NULL));
    for(int i = 0; i < STR_LEN; i++){
       flag = rand()%3;
       switch(flag){
            case 0: salt[i] = rand()%26 + 'a';  break;
            case 1: salt[i] = rand()%26 + 'A';  break;
            case 2: salt[i] = rand()%26 + '0';  break;
       }
    }
    salt[0] = '$';
    salt[1] = '6';
    salt[2] = '$';
    printf("%s\n",salt);
    return 0;
}

/********* 登录*******/
//注册 插入数据
//insert into userInfo(username, salt, cryptpasswd) values ('username', 'salt', 'cryptpasswd')
int insertUserInfo(char *username, char *salt, char *cryptpasswd){
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
        return EXIT_FAILURE;
    }
    else{
        printf("connecting to database\n");
    }
    //查询数据
    char query[128] = {0};
    //insert into table_name (column1, column2, column3) 
    //values (value1, value2, value3);
    sprintf(query, "insert into userInfo(username, salt, cryptpasswd) values ('%s', '%s', '%s')", username, salt, cryptpasswd);
    //执行以NULL结尾的字符串指定的sql查询语句,成功返回0，错误返回非0
    int ret = mysql_query(conn, query);
    if(ret != 0){
        //返回最近调用MYSQL函数发生的错误信息
        const char *errorStr = mysql_error(conn);
        printf("error query %s\n",errorStr);
        return EXIT_FAILURE;
    }else{
        printf("insert into success");
    }
    mysql_close(conn);
    return 0;
}
//查询登录用户名是否存在 return 1 注册 0未注册 -1出错
//已注册取出salt
//select * from userInfo where username = 'username'
int queryUserInfo(char *username, char *salt){//salt传入传出参数
    MYSQL *conn = NULL;//已有MYSQL的ip地址
    MYSQL_RES * res;//结果集的行
    MYSQL_ROW  row;//结果集的字段数(列)
    char host[] = "localhost";//主机名或者ip地址
    char user[] = "root";
    char passwd[] = "123456";
    char database[] = "NetworkDisk";//数据库名称
    //初始化mysql句柄
    conn = mysql_init(NULL);
    //建立连接
    if(mysql_real_connect(conn, host, user, passwd, database, 0, NULL, 0) == NULL){
        printf("connect to database error: %s\n", mysql_error(conn));
        return -1;
    }
    else{
        printf("connect to database success\n");
    }
    //查询数据
    char query[128] = {0};
    //insert into table_name (column1, column2, column3) 
    //values (value1, value2, value3);
    sprintf(query, "select * from userInfo where username = '%s' ",  username );
    puts(query);
    //执行以NULL结尾的字符串指定的sql查询语句,成功返回0，错误返回非0
    int ret = mysql_query(conn, query);
    if(ret != 0){//错误
        //返回最近调用MYSQL函数发生的错误信息
        const char *errorStr = mysql_error(conn);
        printf("error query %s\n",errorStr);
        mysql_close(conn);
        return EXIT_FAILURE;
    }else{//查询成功
         //将查询的全部结果读取到客户端，分配1个MYSQL_RES结构,并将结果置于该结构中
        res  = mysql_store_result(conn);
        if(res == NULL){
            printf("error result %s\n",mysql_error(conn));
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
        //返回结果集的行数
        int rows = mysql_num_rows(res);
        //返回结果集的字段数(列数)
        int cols = mysql_num_fields(res);
        printf("rows: %d, cols: %d\n", rows, cols);
        //获取每一行的数据
        while((row = mysql_fetch_row(res)) != NULL){
            strcpy(salt, row[2]);//取出salt
            //cols-2 不显示密文
            for(int i = 0; i < cols-2; ++i){
                printf("%-10s", row[i]);
            }
            printf("\n");
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 1;
    }
    mysql_close(conn);
    return 0;
}
//查询密码是否正确 return 1正确 0错误 -1出错
//select * from userInfo where cryptpasswd = 'cryptpasswd'
int querrycryptpasswd(char *cryptpasswd){
    MYSQL *conn = NULL;//已有MYSQL的ip地址
    MYSQL_RES * res;//结果集的行
    MYSQL_ROW  row;//结果集的字段数(列)
    char host[] = "localhost";//主机名或者ip地址
    char user[] = "root";
    char passwd[] = "123456";
    char database[] = "NetworkDisk";//数据库名称
    //初始化mysql句柄
    conn = mysql_init(NULL);
    //建立连接
    if(mysql_real_connect(conn, host, user, passwd, database, 0, NULL, 0) == NULL){
        printf("error connecting to database: %s\n", mysql_error(conn));
        return -1;
    }
    else{
        printf("connecting to database\n");
    }
    //查询数据
    char query[128] = {0};
    //insert into table_name (column1, column2, column3) 
    //values (value1, value2, value3);
    sprintf(query, "select * from userInfo where cryptpasswd = '%s' ",  cryptpasswd);
    puts(query);
    //执行以NULL结尾的字符串指定的sql查询语句,成功返回0，错误返回非0
    int ret = mysql_query(conn, query);
    if(ret != 0){//错误
        //返回最近调用MYSQL函数发生的错误信息
        const char *errorStr = mysql_error(conn);
        printf("error query %s\n",errorStr);
        mysql_close(conn);
        return EXIT_FAILURE;
    }else{//查询成功
         //将查询的全部结果读取到客户端，分配1个MYSQL_RES结构,并将结果置于该结构中
        res  = mysql_store_result(conn);
        if(res == NULL){
            printf("error result %s\n",mysql_error(conn));
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
        //返回结果集的行数
        int rows = mysql_num_rows(res);
        //返回结果集的字段数(列数)
        int cols = mysql_num_fields(res);
        printf("rows: %d, cols: %d\n", rows, cols);
        //获取每一行的数据
        while((row = mysql_fetch_row(res)) != NULL){
            //cols-2 不显示密文
            for(int i = 0; i < cols-2; ++i){
                printf("%-10s", row[i]);
            }
            printf("\n");
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 1;
    }
    mysql_close(conn);
    return 0;
}
//*******登录*********
int userLogin(int peerFd, char * username){
    char salt[64] = {0};
    char cryptpasswd[128] = {0};
    //1.先查询此用户是否注册 ret = 1/0/-1 注册/未注册/出错
    int ret = queryUserInfo(username, salt);//已注册取出salt
    printf("queryUserInfo: %d\n", ret);
    //统一字符串，方便发送和接收
    char isRegister[2] = {0};//是否注册标志 1/0 注册/未注册
    char isTruePassword[2] = {0};//密码是否正确标志 1/0 正确/错误
    if(1 == ret){//已注册
        strcpy(isRegister, "1");//
        send(peerFd, isRegister, sizeof(isRegister), 0);//1.1发送已注册信息给客户端
        send(peerFd, salt, sizeof(salt), 0);//1.1.1发送salt 
reEnterPasswd:   //密码错误重新输入
        strcpy(isTruePassword, "1");//重置为1
        recv(peerFd, cryptpasswd, sizeof(cryptpasswd), 0);//1.1.2接收密文
        ret = querrycryptpasswd(cryptpasswd);//密码是否正确标志 1/0 正确/错误
        if(0 == ret){
            strcpy(isTruePassword, "0");
            //1.1.3发送给客户端密码错误重新输入
            send(peerFd, isTruePassword, sizeof(isTruePassword), 0);
            printf("Password is falsei\n");
            goto reEnterPasswd; 
        }
        else{//1.1.3
            send(peerFd, isTruePassword, sizeof(isTruePassword), 0);
            printf("Password is true\n");
        }
        return 0;
    }
    else{//发送未注册信号
        send(peerFd, isRegister, sizeof(isRegister), 0);//1.2发生未注册信息
        generateSalt(salt);//生成salt
        send(peerFd, salt, sizeof(salt), 0);//发送salt
        recv(peerFd, cryptpasswd, sizeof(cryptpasswd), 0);//接收密文
        insertUserInfo(username, salt, cryptpasswd);//将密文写入
    }
    return 0;
}


