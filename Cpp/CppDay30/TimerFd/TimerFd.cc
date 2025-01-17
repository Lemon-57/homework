#include "TimerFd.h"
#include <time.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <poll.h>

#include <iostream>

using std::cout;
using std::endl;

TimerFd::TimerFd(int initsec, int perisec, TimerFdCallback &&cb)
: _timerfd(createTimerFd())
, _initsec(initsec)
, _perisec(perisec)
, _cb(std::move(cb))
, _isStarted(false)
{

}

TimerFd::~TimerFd()
{
    if(_timerfd)
    {
        //设置停止定时器
        setTimerFd(0, 0);
        close(_timerfd);
    }
}

void TimerFd::start()
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = _timerfd;

    setTimerFd(_initsec, _perisec);

    _isStarted = true;
    while(_isStarted)
    {
        int nready = poll(&pfd, 1, 5000);
        cout << "nready = " << nready << endl;

        if(-1 == nready && errno == EINTR)
        {
            continue;
        }
        else if(-1 == nready)
        {
            perror("poll nready == -1");
            return;
        }
        else if(0 == nready)
        {
            cout << "poll  time_out " << endl;
        }
        else
        {
            if(pfd.revents & POLLIN)
            {
                handleRead();//写了read
                if(_cb)
                {
                    _cb();//执行任务
                }
            }
        }
    }
}

void TimerFd::stop()
{
    _isStarted = false;
     setTimerFd(0, 0);
}

void TimerFd::handleRead()
{
    uint64_t one = 1;
    int ret = read(_timerfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read");
        return;
    }
}

int TimerFd::createTimerFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(fd < 0)
    {
        perror("TimerFd");
        return -1;
    }

    return fd;
}

void TimerFd::setTimerFd(int initsec, int perisec)
{
    struct itimerspec value;
    value.it_value.tv_sec = initsec;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = perisec;
    value.it_interval.tv_nsec = 0;

    int fd = timerfd_settime(_timerfd, 0, &value, nullptr);
    if(fd < 0)
    {
        perror("timerfd_settime");
        return ;
    }
}
