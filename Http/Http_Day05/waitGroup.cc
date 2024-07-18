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


int main(int argc, char *argv[])
{
    signal(SIGINT, sigHandler);
    //wait
    waitGroup.wait();
    return 0;
}