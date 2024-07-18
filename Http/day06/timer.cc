#include "unixHeader.h"
#include <workflow/WFFacilities.h>
static WFFacilities::WaitGroup waitGroup(1);
void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}
void callback(WFTimerTask *timerTask){
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    fprintf(stderr,"timer callback time = %ld.%06ld\n", tv.tv_sec, tv.tv_usec);
    auto nextTask =  WFTaskFactory::create_timer_task(5*1000*1000,callback);
    series_of(timerTask)->push_back(nextTask);
}
int main(){
    signal(SIGINT,sigHandler);
    auto timerTask = WFTaskFactory::create_timer_task(5*1000*1000,callback);
    timerTask->start();
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    fprintf(stderr,"timer begin time = %ld.%06ld\n", tv.tv_sec, tv.tv_usec);
    waitGroup.wait();
    return 0;
}
