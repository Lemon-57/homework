#include "TimerFd.h"
#include "Thread.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

#if 0
class MyTask
{
public:
    void process()
    {
        cout << "The process is ruinning" << endl;
    }
};
#endif

void process()
{
    cout << "The process is ruinning" << endl;
}

int main(int argc, char **argv)
{
    TimerFd tf(1, 4, std::bind(process));

    Thread th(std::bind(&TimerFd::start, &tf));
    th.start();//

    sleep(30);

    tf.stop();
    th.join();

    return 0;
}

