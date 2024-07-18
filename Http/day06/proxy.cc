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
struct SeriesContext{
    protocol::HttpResponse *resp;
};
void process(WFHttpTask *serverTask){
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    fprintf(stderr,"process begin time = %ld.%06ld", tv.tv_sec, tv.tv_usec);
    SeriesContext *pContext = new SeriesContext;
    pContext->resp = serverTask->get_resp();//这个指针指向了将要返回给客户端的内容
    series_of(serverTask)->set_context(pContext);
    // 创建一个http任务
    auto httpTask = WFTaskFactory::create_http_task("http://47.94.147.94",0,0,[](WFHttpTask *httpTask){
        // http任务的回调要把响应的内容存储起来
        struct timeval tv;
        gettimeofday(&tv,nullptr);
        fprintf(stderr,"httpTask callback begin time = %ld.%06ld", tv.tv_sec, tv.tv_usec);
        SeriesContext *pContext = static_cast<SeriesContext *>(series_of(httpTask)->get_context());
        // 先拿到backend的响应内容
        const void *body;
        size_t size;
        httpTask->get_resp()->get_parsed_body(&body,&size);
        // 再把响应内容写入到resp
        pContext->resp->append_output_body(body,size);
    });
    // 加入到serverTask所在的序列中
    series_of(serverTask)->push_back(httpTask);
    
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
