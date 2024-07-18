#include<bits/stdc++.h>
#include<workflow/WFFacilities.h>
#include<workflow/WFTaskFactory.h>
#include<workflow/HttpUtil.h>

using namespace std;

//等待一个工作完成
//比如ctrl+c信号后就是一个任务完成
static WFFacilities::WaitGroup waitGroup(1);


void sigHandler(int signo){
    //wake
    fprintf(stderr, "One Work is done\n");
    waitGroup.done();
}

//执行实际，任务的基本工作完成以后
void httpCallback(WFHttpTask * httpTask){
    fprintf(stderr, "base job is done\n");
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigHandler);
    //使用工厂函数创建任务
    //WFHttpTask * httpTask = WFTaskFactory::create_http_task("http://192.168.126.129", 0, 0, httpCallback);
    int i = 1;
    // WFHttpTask * httpTask = WFTaskFactory::create_http_task("http://192.168.126.129", 
    //                                                          0, 
    //                                                          0, 
    //                                                          [i](WFHttpTask *httpTask){fprintf(stderr, "base job is done\nfrom lambda i = %d\n", i);});
    WFHttpTask * httpTask = WFTaskFactory::create_http_task("http://192.168.126.129", 0, 0, 
                                                             [](WFHttpTask *httpTask)
                                                             {
                                                                protocol::HttpRequest * req = httpTask->get_req();
                                                                protocol::HttpResponse * resp = httpTask->get_resp();
                                                                fprintf(stderr, "%s %s %s\r\n",req->get_method(),req->get_http_version(),req->get_request_uri());
                                                                protocol::HttpHeaderCursor reqCursor(req);
                                                                string name;
                                                                string value;
                                                                while(reqCursor.next(name, value)){
                                                                    fprintf(stderr, "%s:%s\r\n", name.c_str(), value.c_str());
                                                                }
                                                             }
                                                           );


    //修改任务属性
    protocol::HttpRequest * rep = httpTask->get_req();
    rep->add_header_pair("key", "value");
    //异步地启动任务
    httpTask->start();
    waitGroup.wait();
    return 0;
}