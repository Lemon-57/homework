#pragma once

#include "Thread.h"
#include "TaskQueue.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;


class Producer
:public Thread
{
public:
    Producer(TaskQueue &taskque) 
    :_taskQue(taskque)
    {

    }
    void run() override{
        ::srand(clock());
        int cnt = 20;
        while (cnt-- >0)
        {
            int number = ::rand() %100;
            _taskQue.push(number);
            cout << ">> Producer number " << number <<endl;
            sleep(1);
        }
    }
    ~Producer() {}
private:
    TaskQueue &_taskQue;
};

