#include"threadPool.h"

//timefd,Linux基于fd的定时接口 通过fd的读事件进行超时通知  
//队列信息中添加exitNode_t结构体数组
//全局变量slot设置定时器 1s触发一次epoll_wait
//建立一个数组存储要退出的进程的pid 数组下标为每个子线程的fd
//触发epoll_wait时遍历数组检测是有要退出的pid
int addTimeFd(int *timeFd){
    struct itimerspec new_value;
    struct timespec cur;
    //获取系统实时时间，随系统时间改变而改变
    clock_gettime(CLOCK_REALTIME,  &cur);
    //it_value表示定时器第一次超时时间
    new_value.it_value.tv_sec = 1;
    new_value.it_value.tv_nsec = cur.tv_nsec;
    //it_interval表示之后超时时间每隔多久超时
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;
    //构建定时器 并返回timefd文件描述符//TFD_NONBLOCK 非阻塞模式
    *timeFd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    //启动定时器
    timerfd_settime(*timeFd, 0, &new_value, NULL);
    
    return 0;
}
