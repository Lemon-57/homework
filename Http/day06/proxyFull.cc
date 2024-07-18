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
    //protocol::HttpResponse *resp;
    WFHttpTask *serverTask;
};
void process(WFHttpTask *serverTask){
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    fprintf(stderr,"process begin time = %ld.%06ld\n", tv.tv_sec, tv.tv_usec);
    SeriesContext *pContext = new SeriesContext;
    pContext->serverTask = serverTask;
    series_of(serverTask)->set_context(pContext);
    // 创建一个http任务
    auto httpTask = WFTaskFactory::create_http_task("http://47.94.147.94",0,0,[](WFHttpTask *httpTask){
        // http任务的回调要把响应的内容存储起来
        struct timeval tv;
        gettimeofday(&tv,nullptr);
        fprintf(stderr,"httpTask callback begin time = %ld.%06ld\n", tv.tv_sec, tv.tv_usec);
        SeriesContext *pContext = static_cast<SeriesContext *>(series_of(httpTask)->get_context());
        auto resp = pContext->serverTask->get_resp();
        auto httpResp = httpTask->get_resp();
        const void *body;
        size_t size;
        httpResp->get_parsed_body(&body, &size);//找到报文体
        httpResp->append_output_body(body, size);//把报文体的内容拼接到resp中
        *resp = std::move(*httpResp);
        // pContext->serverTask->set_callback([](WFHttpTask *serverTask){
        //     struct timeval tv;
        //     gettimeofday(&tv,nullptr);
        //     fprintf(stderr,"serverTask callback begin time = %ld.%06ld\n", tv.tv_sec, tv.tv_usec);
        //     SeriesContext *pContext = static_cast<SeriesContext *>(series_of(serverTask)->get_context());
        //     delete pContext;
        // });
    });
    // 加入到serverTask所在的序列中
    series_of(serverTask)->push_back(httpTask);
    pContext->serverTask->set_callback([](WFHttpTask *serverTask){
        struct timeval tv;
        gettimeofday(&tv,nullptr);
        fprintf(stderr,"serverTask callback begin time = %ld.%06ld\n", tv.tv_sec, tv.tv_usec);
        SeriesContext *pContext = static_cast<SeriesContext *>(series_of(serverTask)->get_context());
        delete pContext;
    });   
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
