#include "unixHeader.h"
#include <string>
#include <workflow/Workflow.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/WFFacilities.h>

using namespace protocol;
using std::string;
using std::vector;

//所有要共享的信息
struct SeriesContext
{
    string url;
    int state;
    int error;
    HttpResponse resp;    
};

//ParallelWork并行任务类
void callbakc(const ParallelWork *pwork){
    SeriesContext *context;
    const void * body;
    size_t size;
    size_t i;
    for(i = 0; i < pwork->size(); i++){
        //通过series_at获取并行任务中每个的队列的详细信息
        context = (SeriesContext *)pwork->series_at(i)->get_context();
        printf("%s\n", context->url.c_str());
        if(context->state == WFT_STATE_SUCCESS){
            //收集前面获取的响应内容，并且释放所有已经申请的内存
            context->resp.get_parsed_body(&body, &size);//获取解析过的body
            printf("%zu%s\n", size, context->resp.is_chunked() ? "chunked" : "");
            fwrite(body, 1, size, stdout);//将http响应信息写入到SeriesContext对象中
            printf("\n");
        }
        else{
            printf("ERROR! state = %d, error = %d\n", context->state, context->error);
        }
        delete context;
    }
}

int main(){
    //1.首先创建了一个 ParallelWork 类型的并行任务；
    //并行任务本身的回调函数，它通过context获取了每个序列的执行结果，然后释放了context
    ParallelWork *pwork = Workflow::create_parallel_work(callbakc);
    SeriesWork * series;
    WFHttpTask *task;
    HttpRequest *req;
 
    SeriesContext *context;
    //2.根据三个url，创建三个http任务
    vector<string> urlvectior = {"http://www.baidu.com", "https://47.94.147.94", "http://192.168.126.129"};
    for(size_t i = 0; i < urlvectior.size(); ++i){
        string url = urlvectior[i];
        task = WFTaskFactory::create_http_task(url,0,0,[](WFHttpTask *task){
            //创建一个值语义的类SeriesContext，用来含括所有要共享的信息；
            SeriesContext *context = (SeriesContext *)series_of(task)->get_context();
            context->state = task->get_state();
            context->error = task->get_error();
            context->resp = std::move(*task->get_resp());//没有拷贝构造函数
        });
        req = task->get_req();
        req->add_header_pair("Accept", "*/*");
        req->add_header_pair("User-Agent", "Wget/1.14(linux-gnu)");
        req->add_header_pair("Connection", "close");
        //为SeriesContext对象申请内存空间保存状态，并且作为该序列的context；
        //这个ctx的功能和之前的user_data的功能是一样的，都是为了让在同一个序列的多个任务来共享数据）
        context = new SeriesContext;
        //并根据每个http任务创建一个序列   
        series = Workflow::create_series_work(task, nullptr);
        series->set_context(context);
        //创建一个并行任务，然后将三个序列都加入到并行任务
        //并且为该并行任务设置一个回调函数
        pwork->add_series(series);
    }
    WFFacilities::WaitGroup wait_group(1);
    //4.最后调用了Workflow::start_series_work，这样并行任务本身就又以序列的方式启动了。
    Workflow::start_series_work(pwork, [&wait_group](const SeriesWork *){
        wait_group.done();
    });
    wait_group.wait();
    return 0;
}