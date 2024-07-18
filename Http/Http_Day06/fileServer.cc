#include "unixHeader.h"
#include <workflow/WFFacilities.h>
#include <workflow/WFHttpServer.h>
#include <workflow/HttpUtil.h>
static WFFacilities::WaitGroup waitGroup(1);
void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}
void process(WFHttpTask *serverTask){
    auto resp = serverTask->get_resp();
    auto req = serverTask->get_req();
    std::string method = req->get_method();
    if(method == "GET"){
        char *buf = new char[614];
        auto fileIOTask = WFTaskFactory::create_pread_task("postform.html", buf, 614, 0, [resp,buf](WFFileIOTask *fileIOTask){
            resp->add_header_pair("Content-Type","text/html");
            resp->append_output_body(buf, 614);
        });
        series_of(serverTask)->push_back(fileIOTask);
        serverTask->set_callback([buf](WFHttpTask *serverTask){
            delete[] buf;
        });
    }
    else if(method == "POST"){
        const void *body;
        size_t size;
        req->get_parsed_body(&body,&size);
        std::string wholeBody = static_cast<const char *>(body);
        //"username=admin&password=admin"
        std::string usernameKV = wholeBody.substr(0,wholeBody.find("&"));
        std::string passwordKV = wholeBody.substr(wholeBody.find("&")+1);
        //"username=admin"
        std::string username = usernameKV.substr(usernameKV.find("=")+1);
        std::string password = passwordKV.substr(passwordKV.find("=")+1);
        //boost::string::split
        if(username == "admin" && password == "admin"){
            resp->append_output_body_nocopy("SUCCESS",7);
            resp->add_header_pair("Content-Type","text/plain");
        }
        else{
            resp->append_output_body_nocopy("FAIL",4);
            resp->add_header_pair("Content-Type","text/plain");
        }
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
