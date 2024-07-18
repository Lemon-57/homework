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

//共享数据:  1.创建任务的函数和任务的回调函数中user_data
//           2.通过序列的contex set_context get_context
int main(int argc, char *argv[])
{
    signal(SIGINT, sigHandler);
    string key = "44key";
    //step2 创建任务1 设置基本工作 和 回调函数
    auto redisTask1 = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0, [](WFRedisTask *redisTask1)
        {
            //step 5 执行第1个任务的回调函数中创建了新的任务2
            //step 6 创建任务2
            auto redisTask2 = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379", 0, [](WFRedisTask *redisTask2)
                {
                    //step 10 执行任务2的回调
                    string *pkey = static_cast<string *>(series_of(redisTask2)->get_context());
                    fprintf(stderr, "redisTask2 callback begins!, *pkey = %s\n", pkey->c_str());
                    auto resp = redisTask2->get_resp();
                    protocol::RedisValue value;
                    resp->get_result(value);
                    cout <<value.string_value().c_str() << endl;
                    if(value.is_string()){
                        fprintf(stderr, "value = %s\n", value.string_value().c_str());
                    }
                }
            
            );
            //step 7设置任务2的属性
            string *pkey = static_cast<string *>(series_of(redisTask1)->get_context());
            auto req = redisTask2->get_req();
            //设置redis命令 建立连接的过程由框架自动完成
            req->set_request("GET", {*pkey});
            //step 8 将任务2放入序列中
            //序列 一串串行的任务

            //任务1--->任务2--->任务3
            //task1--->base ---->task1-->callback--->task2->base--->task2-->callback--->task3->base--->task3-->callback
            series_of(redisTask1)->push_back(redisTask2);
            fprintf(stderr, "redisTask1 callback is done!\n");
            //step 9 任务1的回调执行完了，开始执行序列中下一个任务
            
        }
    );
    //step 2 设置任务1的属性 先找到
    auto req = redisTask1->get_req();
    req->set_request("SET", {key, "100"});
    //redisTask1->start();
    //step 3 根据任务创建序列，启动序列 线程和序列并发执行
    auto series = Workflow::create_series_work(redisTask1, nullptr);
    series->set_context(&key);
    //step 4 任务1开始执行
    series->start();
    waitGroup.wait();
    return 0;
}

