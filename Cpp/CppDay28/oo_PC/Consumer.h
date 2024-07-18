#pragma once

#include "Thread.h"
#include "TaskQueue.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;


class Consumer
:public Thread
{
public:
    Consumer(TaskQueue &taskque) 
    :_taskQue(taskque)
    {

    }
    void run() override{
        // ::srand(clock());
        // int cnt = 20;
        // while (cnt-- >0)
        while(1)
        {
            // int number = ::rand() %100;
            // _taskQue.push(number);
            int tmp = _taskQue.pop();

            cout << ">> Consumer number " << tmp <<endl;
            sleep(1);
        }
    }
    ~Consumer() {}
private:
    TaskQueue &_taskQue;
};

