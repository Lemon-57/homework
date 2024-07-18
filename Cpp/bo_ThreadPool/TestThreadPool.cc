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
{
public:
    // d + 目标行数 + G
    // y + 目标行数 + G
    // ngcc注释多行
    void process()
    {
        ::srand(clock());
        int number = ::rand() % 100;
        cout << ">>bo_ThreadPool.number = " << number << endl;
    }
};

void func()
{
    ::srand(clock());
    int number = ::rand() % 100;
    cout << ">>bo_ThreadPool1.number = " << number << endl;
}

int main(int argc, char **argv)
{
    MyTask task;
    ThreadPool pool(4, 10);
    pool.start();

    int cnt = 20;
    while (cnt-- > 0)
    {
        pool.addTask(bind(&MyTask::process, &task)); // 传地址与非地址都ok
        /* pool.addTask(bind(func));//传地址与非地址都ok */
        cout << "cnt = " << cnt << endl;
    }

    pool.stop();

    return 0;
}
