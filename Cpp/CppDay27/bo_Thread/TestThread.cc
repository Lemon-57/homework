#include <iostream>
#include<unistd.h>
#include<memory>
#include"Thread.h"

using std::cout;
using std::endl;
using std::bind;
using std::unique_ptr;

class Mytask
{
public:
    void process() {
        //具体的任务根据需求进行完善即可
        //...
        while (1)
        {
            cout << "The thread is running1!" <<endl;
            sleep(1);
        }
        
    }
};

void func(int x){
    //具体的任务根据需求进行完善即可
    //...
    while (1)
    {
        cout << "The thread is running!" <<endl;
        sleep(x);
    }    
}

void test(){
    Mytask task;
    //bind可以改变函数的形态
    function<void()> f = (bind(&Mytask::process, &task)); 
    Thread th(std::move(f));
    th.start();
    //主线程(main主线程)等子线程退出
    th.join();
}

int main(int argc, char **argv)
{
    // test();
    //bind可以改变函数的形态
    Thread th(std::move(bind(&func, 5)));
    th.start();
    //主线程(main主线程)等子线程退出
    th.join();
    return 0;
}

