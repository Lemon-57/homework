#include "ThreadPool.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class MyTask
: public Task
{
public:
    void process() override
    {
        ::srand(clock());
        int number = ::rand() % 100;
        cout << "number = " << number << endl;
        /* sleep(1); */
    }
};

int main(int argc, char **argv)
{
    unique_ptr<Task> ptask(new MyTask());
    ThreadPool pool(4, 10);
    pool.start();

    int cnt = 20;
    while(cnt-- > 0)
    {
        pool.addTask(ptask.get());
        cout << "cnt = " << cnt << endl;
        /* sleep(1); */
        
    }

    pool.stop();

    return 0;
}

