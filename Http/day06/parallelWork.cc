#include "unixHeader.h"
#include <workflow/WFFacilities.h>
#include <workflow/HttpUtil.h>
static WFFacilities::WaitGroup waitGroup(1);
void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}
struct SeriesContext{
    protocol::HttpResponse resp;
};
void callback(const ParallelWork *pwork){
    fprintf(stderr,"ParallelWork Callback!\n");
    for(unsigned int i = 0; i < pwork->size(); ++i){
        SeriesContext * pContext = static_cast<SeriesContext *>(pwork->series_at(i)->get_context());
        const void *body;
        size_t size;
        pContext->resp.get_parsed_body(&body,&size);
        fwrite(body,1,size,stderr);
        fprintf(stderr,"\n");
        delete pContext;
    }
}
int main(){
    signal(SIGINT,sigHandler);
    ParallelWork *pwork = Workflow::create_parallel_work(callback);
    std::vector<std::string> urlVec = {"http://www.baidu.com", "http://47.94.147.94", "http://localhost"};
    for(int i = 0; i < 3; ++i){
        //创建任务
        auto httpTask = WFTaskFactory::create_http_task(urlVec[i],0,0,[](WFHttpTask *httpTask){
            SeriesContext * pContext = static_cast<SeriesContext *>(series_of(httpTask)->get_context());
            auto resp = httpTask->get_resp();
            pContext->resp = std::move(*resp);
            //pContext->resp = *resp;
            // protocol::HttpHeaderCursor respCursor(resp);
            // std::string key;
            // std::string value;
            // while(respCursor.next(key,value)){
            //     fprintf(stderr,"task callback, %s:%s\n", key.c_str(), value.c_str());
            // }
            // fprintf(stderr,"\n");
        });
        //创建序列
        auto series = Workflow::create_series_work(httpTask,nullptr);
        SeriesContext * pContext = new SeriesContext;
        series->set_context(pContext);
        //把序列加入到并行任务中
        pwork->add_series(series);
    }
    pwork->start();
    waitGroup.wait();
    return 0;
}
