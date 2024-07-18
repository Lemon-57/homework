#include<bits/stdc++.h>
#include<workflow/WFFacilities.h>
#include<workflow/WFTaskFactory.h>

using namespace std;

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
    //使用工厂函数创建任务
    WFHttpTask * httpTask = WFTaskFactory::create_http_task("http://192.168.126.129", 0, 0, nullptr);
    //修改任务属性
    protocol::HttpRequest * rep = httpTask->get_req();
    rep->add_header_pair("key", "value");
    //异步地启动任务
    httpTask->start();
    waitGroup.wait();
    return 0;
}