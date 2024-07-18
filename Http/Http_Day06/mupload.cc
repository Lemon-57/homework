#include "unixHeader.h"
#include <workflow/WFFacilities.h>
#include <workflow/WFHttpServer.h>
#include <workflow/HttpUtil.h>
#include <nlohmann/json.hpp>
using Json = nlohmann::json;
using std::string;


static WFFacilities::WaitGroup waitGroup(1);
void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}
void process(WFHttpTask *serverTask){
    // 1 解析用户的请求
    auto req = serverTask->get_req();
    auto resp = serverTask->get_resp();
    std::string method = req->get_method();
    std::string uri = req->get_request_uri();
    //fprintf(stderr,"method = %s, uri = %s\n", method.c_str(), uri.c_str());
    std::string path = uri.substr(0,uri.find("?"));
    std::string query = uri.substr(uri.find("?")+1);
    //fprintf(stderr,"path = %s, query = %s\n", path.c_str(), query.c_str());
    // 2 选择一个合适的分支
    if(path == "/file/mupload/init" && method == "POST"){
        // POST /file/mupload/init body:{xxx:xxx}
        // 3 取出响应体
        const void *body;
        size_t size;
        req->get_parsed_body(&body,&size);
        // 4 将响应体解析成json
        Json js = Json::parse(static_cast<const char *>(body));
        std::string filename = js["filename"];
        std::string filehash = js["filehash"];
        int filesize = js["filesize"];
        // 5 生成分片信息
        // uploadID
        time_t now = time(nullptr);
        struct tm* ptm = localtime(&now);
        char uploadID[256];
        snprintf(uploadID,256,"liao%02d:%02d",ptm->tm_hour,ptm->tm_min);
        fprintf(stderr,"uploadID = %s\n", uploadID);
        // 分片的信息
        int chunksize = 1024*1024;
        int chunkcount;
        if(filesize > chunksize){
            chunkcount = (filesize/chunksize) + (filesize%chunksize != 0);
        }
        // 6 构建给客户端回复的内容
        Json respJs;
        respJs["uploadID"] = uploadID;
        respJs["chunkcount"] = chunkcount;
        respJs["chunksize"] = chunksize;
        respJs["filesize"] = filesize;
        respJs["filehash"] = filehash;
        resp->append_output_body(respJs.dump());

        // 7 将信息写入redis
        std::string filesizestr = std::to_string(filesize);
        std::string chunkcountstr = std::to_string(chunkcount);
        std::vector<std::vector<std::string>> argsVec = {
            {uploadID,"chunkcount", chunkcountstr},
            {uploadID,"filesize", filesizestr},
            {uploadID,"filehash", filehash}
        };
        for(int i = 0; i < 3; ++i){
            auto redisTask = WFTaskFactory::create_redis_task("redis://localhost",0,nullptr);
            redisTask->get_req()->set_request("HSET",argsVec[i]);
            series_of(serverTask)->push_back(redisTask);
        }
    }
    else if(path == "/file/mupload/upload" && method == "POST"){
        //1.解析查询字符串
        //"uploadID=liao09:53&chkidx=1"
         string uploadIDKV = query.substr(0, query.find("&"));
         string chkidxKV  = query.substr(query.find("&")+1);
         string uploadID = uploadIDKV.substr(uploadIDKV.find("="+1));
         string chkidx = chkidxKV.substr(chkidxKV.find("="+1));
        //fprintf();
        //2.读取报文体，保存为文件
        //
        //3.存储缓存信息
    }
    else if(path == "/file/mupload/complete" && method == "GET"){

    }
    else{
        serverTask->get_resp()->set_status_code("500");
        return;
    }
}
int main(){
    signal(SIGINT,sigHandler);
    WFHttpServer server(process);
    //process是一个函数对象
    //当客户端接入时，框架会创建一个服务端任务，自动调用process
    //用户可以通过自定义process，定位到服务端任务

    if(server.start(1234) == 0){
        waitGroup.wait();
        server.stop();
    }
    else{
        perror("server start:");
        exit(-1);
    }
    return 0;
}
