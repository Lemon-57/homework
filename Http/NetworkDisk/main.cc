#include "unixHeader.h"
#include "hashValue.h"
#include <bits/stdc++.h>
#include <workflow/WFFacilities.h>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLUtil.h>
#include <wfrest/HttpServer.h>
#include <workflow/MySQLResult.h>

using std::string;
using std::map;
using std::vector;
using std::pair;
using std::to_string;

//等待一个工作完成
//比如ctrl+c信号后就是一个任务完成
static WFFacilities::WaitGroup waitGroup(1);


void sigHandler(int signo){
    //wake
    fprintf(stderr, "One Work is done\n");
    waitGroup.done();
}


int main(int argc, char *argv[])
{
    signal(SIGINT, sigHandler);
    wfrest::HttpServer server;
    server.GET("/file/upload", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/index.html");
    });
    server.POST("/file/upload", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        //1 解析用户的post from-data
        using Form = map<string, pair<string, string>>;
        Form form = req->form();
        //2 取出文件的名字和内容 保存到服务端文件系统中
        auto filename = form["file"].first;
        string filepath = "tmp/" + filename;
        auto filecontent = form["file"].second;
        //fprintf(stderr, "filepath = %s\n, filecontent = %s\n" , filepath.c_str(), filecontent.c_str());
        int fd = open(filepath.c_str(), O_RDWR|O_CREAT, 0666);
        write(fd, filecontent.c_str(), filecontent.size());
        close(fd);
        //3 将文件的元信息写入数据库
        HashValue filepathcalc(filepath);
        string filehash = filepathcalc.getsha1sum();
        string filesizestr = to_string(filecontent.size());
        string sql = "INSERT INTO cloundisk.tbl_file(file_sha1, file_name, file_size, file_addr) VALUES('" 
                     + filehash + "','"+filename + "','" + filesizestr + "','" + filepath + "');'";
        fprintf(stderr, "sql = %s\n", sql.c_str());
        string uri = "mysql://root:123456@localhost";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, nullptr);
        mysqlTask->get_req()->set_query(sql);    
        series->push_back(mysqlTask);
        //4 响应客户端一个上传完成的页面 重定向
        resp->set_status_code("301");
        resp->headers["Location"] = "file/upload/success";
    });
    server.GET("/file/upload/ success", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->String("Upload success");
    });
    
    server.GET("/file/download", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        //1 提取文件的信息
        auto fileInfo = req->query_list();
        string sha1sum = fileInfo["filehash"];
        string filename = fileInfo["filename"];
        //2 生成一个301 302响应
        resp->set_status_code("301");
        resp->headers["Location"] = "http://192.168.126.129:1235" + filename;
    });

    server.GET("user/signup", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/singup.html");
    });
    server.POST("/user/signup", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork * series){
        //1 解析用户的请求 urlencoded
        auto formKV = req->form_kv();
        string username = formKV["username"];
        string password = formKV["password"];
        //2 将用户的密码加密
        string salt = "123456";
        const char * encodedPassword = crypt(password.c_str(), salt.c_str());
        //3 插入tbl_user表中
        string uri = "mysql://root:123456@localhost";
        string sql = "INSERT INTO cloundisk.tbl_user(user_name, user_pwd) VALUES('" 
                      + username + "','" + encodedPassword + "');'";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, [resp](WFMySQLTask *mysqlTask){
            if(mysqlTask->get_state() != WFT_STATE_SUCCESS){
                fprintf(stderr, "error msg: %s\n", WFGlobal::get_error_string(mysqlTask->get_state(), mysqlTask->get_error()));
                resp->String("FAIL");
                return;
            }
            auto resp4sql = mysqlTask->get_resp();
            if(resp4sql->get_packet_type() != MYSQL_PACKET_ERROR){
                resp->String("FAIL");
                return;
            }
            protocol::MySQLResultCursor cursor(resp4sql);
            if(1 == cursor.get_affected_rows()){
                //成功
                resp->String("SUCCESS");
            }else{
                //失败
                resp->String("FAIL");
            }
        });
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });
    server.GET("/static/view/signin.html", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        resp->File("static/view/signin.html");
    });
    server.POST("/user/signin", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        //1 解析用户目录
        auto formKV = req->form_kv();
        string username = formKV["username"];
        string password = formKV["password"];
        string salt = "123456";
        const char * encodedPassword = crypt(password.c_str(), salt.c_str());
        //2 通过用户名查到密文密码 校验
        string uri = "mysql://root:123456@localhost";
        string sql = "SELECT user_pwd FROM cloundisk.tbl_user WHERE user_name = '" + username +  "' LIMUT 1;";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri, 0, [encodedPassword, resp](WFMySQLTask *mysqlTask){
            auto resp4mysql = mysqlTask->get_resp();
            protocol::MySQLResultCursor cursor(resp4mysql);
            vector<vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);
            string storeEncodedPassword = rows[0][0].as_string();
            fprintf(stderr, "storEncodePasswd = %s\n encodedPassword = %s\n", storeEncodedPassword.c_str(), encodedPassword);
            if(storeEncodedPassword != encodedPassword){
                resp->String("FAIL");
                return;
            }
        });
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });
    if(0 == server.track().start(1234)){
        server.list_routes();
        waitGroup.wait();
        server.stop();
    }else{
        perror("server start");
        exit(-1);
    }

    return 0;
}