#include<bits/stdc++.h>
#include<workflow/WFFacilities.h>

using namespace std;

//等待一个工作完成
//比如ctrl+c信号后就是一个任务完成
static WFFacilities::WaitGroup waitGroup(1);


void sigHandler(int signo){
    //wake
    fprintf(stderr, "One Work is done\n");
    waitGroup.done();
}

struct SeriesContext{
    std::string info;
};

int main(int argc, char *argv[])
{
    signal(SIGINT, sigHandler);
    auto redisTask = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0, [](WFRedisTask *redisTask)
    {
        SeriesContext * pContext = static_cast<SeriesContext *>(series_of(redisTask)->get_context());
        fprintf(stderr, "info = %s\n", pContext->info.c_str());  
    });
    auto req = redisTask->get_req();
    req->set_request("SET", {"x1", "100"});
    auto series = Workflow::create_series_work(redisTask, nullptr);
    SeriesContext * pContext = new SeriesContext;
    pContext->info = "hello";
    series->set_context(pContext);
    series->set_callback([pContext](const SeriesWork * series){
        SeriesContext *pContext = static_cast<SeriesContext *>(series->get_context());
        fprintf(stderr, "series callback, info = %s\n", pContext->info.c_str());
        delete pContext;
    });
    //step 4 任务1开始执行
    series->start();
    waitGroup.wait();
    return 0;
}
