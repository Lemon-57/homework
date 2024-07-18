#include<bits/stdc++.h>
#include <workflow/WFFacilities.h>
static WFFacilities::WaitGroup waitGroup(1);
void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}
int main(){
    signal(SIGINT,sigHandler);
    std::string key = "s1";
    //step 1 创建任务1
    auto redisTask1 = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379",0,[](WFRedisTask *redisTask1){
        auto redisTask2 = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379",0,[](WFRedisTask *redisTask2){
            auto redisTask3 = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379",0,[](WFRedisTask *redisTask3){
                auto redisTask4 = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379",0,[](WFRedisTask *redisTask4){
                    std::string *pkey = static_cast<std::string *>(series_of(redisTask4)->get_context());
                    fprintf(stderr,"redisTask4 callback begins!, *pkey = %s\n", pkey->c_str());
                    auto resp = redisTask4->get_resp();
                    protocol::RedisValue value;
                    resp->get_result(value);
                    if(value.is_string()){
                        fprintf(stderr,"value = %s\n", value.string_value().c_str());
                    }
                });
                std::string *pkey = static_cast<std::string *>(series_of(redisTask3)->get_context());
                auto req = redisTask4->get_req();
                req->set_request("GET",{*pkey});
                series_of(redisTask3)->push_back(redisTask4);
                fprintf(stderr,"redisTask1 callback is done!\n");
            });
            std::string *pkey = static_cast<std::string *>(series_of(redisTask2)->get_context());
            auto req = redisTask3->get_req();
            req->set_request("GET",{*pkey});
            series_of(redisTask2)->push_back(redisTask3);
            fprintf(stderr,"redisTask1 callback is done!\n");
        });
        std::string *pkey = static_cast<std::string *>(series_of(redisTask1)->get_context());
        auto req = redisTask2->get_req();
        req->set_request("GET",{*pkey});
        series_of(redisTask1)->push_back(redisTask2);
        fprintf(stderr,"redisTask1 callback is done!\n");

    });
    auto req = redisTask1->get_req();
    req->set_request("SET",{key,"100"});
    auto series = Workflow::create_series_work(redisTask1,nullptr);
    series->set_context(&key);//key存入序列的context当中
    series->start();
    
    waitGroup.wait();
    return 0;
}
