#include "unixHeader.h"
#include <workflow/WFFacilities.h>
static WFFacilities::WaitGroup waitGroup(1);
void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}
struct SeriesContext{
    std::string info;
};
int main(){
    signal(SIGINT,sigHandler);
    // wait

    auto redisTask = WFTaskFactory::create_redis_task("redis://localhost",0,[](WFRedisTask *redisTask){
        SeriesContext *pContext = static_cast<SeriesContext *>(series_of(redisTask)->get_context());
        fprintf(stderr,"info = %s\n", pContext->info.c_str());
        //delete pContext;
    });
    redisTask->get_req()->set_request("SET",{"x1", "100"});
    auto series = Workflow::create_series_work(redisTask,nullptr);
    SeriesContext *pContext = new SeriesContext;
    pContext->info = "hello";
    series->set_context(pContext);
    //series->set_callback([pContext](const SeriesWork *series){
        //fprintf(stderr,"series callback, info = %s\n", pContext->info.c_str());
    series->set_callback([](const SeriesWork *series){
        SeriesContext *pContext = static_cast<SeriesContext *>(series->get_context());
        fprintf(stderr,"series callback, info = %s\n", pContext->info.c_str());
        delete pContext;
    });
    series->start();
    waitGroup.wait();
    return 0;
}
