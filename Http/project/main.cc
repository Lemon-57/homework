#include "unixHeader.h"
#include "hashValue.h"
#include <workflow/WFFacilities.h>
#include <wfrest/HttpServer.h>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>
static WFFacilities::WaitGroup waitGroup(1);
void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}
int main(){
    signal(SIGINT,sigHandler);
    wfrest::HttpServer server;
    server.GET("/file/upload",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/index.html");
    });
    server.POST("/file/upload",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        //1 解析用户的post form-data请求
        using Form = std::map<std::string, std::pair<std::string, std::string>>;
        Form form = req->form();
        //2 取出文件的名字和内容 保存到服务端文件系统中
        auto filename = form["file"].first;
        std::string filepath = "tmp/" + filename;
        auto filecontent = form["file"].second; 
        //fprintf(stderr,"filepath = %s\n, filecontent = %s\n", filepath.c_str(), filecontent.c_str());
        int fd = open(filepath.c_str(),O_RDWR|O_CREAT,0666);
        write(fd,filecontent.c_str(),filecontent.size());
        close(fd);
        //3 将文件的元信息写入数据库
        HashValue filehashcalc(filepath);
        std::string filehash = filehashcalc.getsha1sum();
        std::string filesizestr = std::to_string(filecontent.size());
        std::string sql = "INSERT INTO cloudisk.tbl_file (file_sha1, file_name, file_size, file_addr) VALUES ('"
            + filehash + "','" + filename + "'," + filesizestr + ", '" + filepath + "');";
        fprintf(stderr,"sql = %s\n", sql.c_str());
        std::string uri = "mysql://root:123@localhost";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri,0,nullptr);
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
        //4 响应客户端一个上传完成的页面 重定向
        resp->set_status_code("301");
        resp->headers["Location"] = "/file/upload/success";
    });
    server.GET("/file/upload/success",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->String("Upload success");
    });

    server.GET("/file/download",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        // // 1 提取文件的信息
        // auto fileInfo = req->query_list();
        // std::string sha1sum = fileInfo["filehash"];
        // std::string filename = fileInfo["filename"];
        // int filesize = std::stoi(fileInfo["filesize"]);
        // // 2 查询文件元信息表格
        // // 3 获取文件在服务端的路径，打开文件
        // std::string filePath = "tmp/" + filename;
        // int fd = open(filePath.c_str(),O_RDONLY);
        // char *buf = new char[filesize];
        // read(fd,buf,filesize);
        // close(fd);
        // // 4 响应回复给客户端
        // resp->append_output_body(buf,filesize);
        // resp->headers["Content-Type"] = "application/octect-stream";
        // resp->headers["content-disposition"] = "attachment;filename="+filename;
        // delete[] buf;

        // 1 提取文件的信息
        auto fileInfo = req->query_list();
        std::string sha1sum = fileInfo["filehash"];
        std::string filename = fileInfo["filename"];
        // 2 生成一个301 302 响应
        resp->set_status_code("301");
        resp->headers["Location"] = "http://192.168.135.129:1235/" + filename;
    });
    server.GET("/user/signup",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/signup.html");
    });
    server.POST("/user/signup",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户的请求 urlencoded
        auto formKV = req->form_kv();
        std::string username = formKV["username"];
        std::string password = formKV["password"];
        // 2 将用户的密码加密
        std::string salt = "12345678";
        const char * encodedPassword = crypt(password.c_str(),salt.c_str());
        // 3 插入tbl_user表中
        std::string uri = "mysql://root:123@localhost";
        std::string sql = "INSERT INTO cloudisk.tbl_user (user_name,user_pwd) VALUES ('"+ 
            username + "', '" + encodedPassword + "');";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri,0,[resp](WFMySQLTask *mysqlTask){
            if(mysqlTask->get_state() != WFT_STATE_SUCCESS){
                fprintf(stderr,"error msg: %s\n",WFGlobal::get_error_string(mysqlTask->get_state()
                                                                   ,mysqlTask->get_error()));
                //resp->set_status_code("500");
                resp->String("FAIL");
                return;
            }
            auto resp4sql = mysqlTask->get_resp();
            if(resp4sql->get_packet_type() == MYSQL_PACKET_ERROR){
                fprintf(stderr,"ERROR, error_code = %d, error_msg = %s\n",
                    resp4sql->get_error_code(),
                    resp4sql->get_error_msg().c_str()
                    );
                //resp->set_status_code("500");
                resp->String("FAIL");
                return;
            }  
            protocol::MySQLResultCursor cursor(resp4sql);
            if(cursor.get_affected_rows() == 1){
                //成功
                resp->String("SUCCESS");
            }
            else{
                //失败
                resp->String("FAIL");
            }
        });
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
    });
    server.GET("/static/view/signin.html",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        resp->File("static/view/signin.html");
    });
    server.POST("/user/signin",[](const wfrest::HttpReq *req, wfrest::HttpResp *resp, SeriesWork *series){
        // 1 解析用户请求
        auto formKV = req->form_kv();
        std::string username = formKV["username"];
        std::string password = formKV["password"];
        std::string salt = "12345678";
        const char * encodedPassword = crypt(password.c_str(),salt.c_str());
        // 2 通过用户名查到密文密码，校验
        std::string uri = "mysql://root:123@localhost";
        std::string sql = "SELECT user_pwd FROM cloudisk.tbl_user WHERE user_name = '" + username + "' LIMIT 1;";
        auto mysqlTask = WFTaskFactory::create_mysql_task(uri,0,[encodedPassword,resp](WFMySQLTask *mysqlTask){
            auto resp4mysql = mysqlTask->get_resp();
            protocol::MySQLResultCursor cursor(resp4mysql);
            std::vector<std::vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);
            std::string storeEncodedPassword = rows[0][0].as_string(); 
            fprintf(stderr,"storeEncodedPassword = %s\n encodedPassword = %s\n",storeEncodedPassword.c_str(),encodedPassword);
            if(storeEncodedPassword != encodedPassword){
                resp->String("FAIL");
                return;
            }
        });
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask);
        // 3 生成token
    });
    if(server.track().start(1234) == 0){
        server.list_routes();
        waitGroup.wait();
        server.stop();
    }
    else{
        perror("server start:");
        exit(-1);
    }
    return 0;
}
