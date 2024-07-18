#include "Thread.h"
#include <unistd.h>
#include <iostream>
#include <memory>

using std::endl;
using std::cout;
using std::unique_ptr;

class Mythread
:public Thread
{
    void run() override{
        //具体的任务根据需求进行完善即可
        //...
        while (1)
        {
            cout << "The thread is running!" <<endl;
            sleep(1);
        }
        
    }
};

int main(void)
{
    unique_ptr<Thread> pth(new Mythread());
    pth->start();
    //主线程(main主线程)等子线程退出
    pth->join();
    return 0;
}

