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
    // serverTask就是框架创建的服务端任务的指针
    auto req = serverTask->get_req();//请求已经接受并解析完成
    auto resp = serverTask->get_resp();//响应还未构建

    protocol::HttpHeaderCursor reqCursor(req);
    char buf[8192] = {0};
    int len;
    resp->append_output_body_nocopy("<html>",6);
    len = snprintf(buf,sizeof(buf),"<p> %s %s %s</p>",req->get_method(),
                                                req->get_request_uri(), 
                                                req->get_http_version());
    resp->append_output_body(buf,len);
    std::string key;
    std::string value;
    while(reqCursor.next(key,value)){
        len = snprintf(buf,sizeof(buf),"<p>%s : %s </p>", key.c_str(), value.c_str());
        resp->append_output_body(buf,len);
    }
    resp->append_output_body_nocopy("</html>",7);

    resp->set_http_version("HTTP/1.1");
    resp->set_status_code("200");
    resp->set_reason_phrase("OK");
    resp->add_header_pair("Content-Type", "text/html");

    struct sockaddr_in addr;
    socklen_t socklen = sizeof(addr);
    serverTask->get_peer_addr((struct sockaddr*)&addr, &socklen);
    fprintf(stderr, "ip = %s, port = %d\n", inet_ntoa(addr.sin_addr) ,ntohs(addr.sin_port));
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
