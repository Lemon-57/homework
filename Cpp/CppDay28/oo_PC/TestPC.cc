#include"Producer.h"
#include"Consumer.h"
#include <iostream>
#include <memory>

using std::endl;
using std::cout;
using std::unique_ptr;

void test(){
    TaskQueue taskQueue(10);
    unique_ptr<Producer>pro1(new Producer(taskQueue));
    unique_ptr<Producer>pro2(new Producer(taskQueue));
    unique_ptr<Consumer>con1(new Consumer(taskQueue));

    pro1->start();
    pro2->start();
    con1->start();

    pro1->join();
    pro1->join();
    con1->join();


    MutexLock mutex;
    puts("1");
    //对象语义 不能进行复制或者赋值
    //MutexLock mutex2 = mutex; //error
    MutexLock mutex3;
    //mutex3 = mutex; //error

    //值语义 可以进行复制或者赋值
}


int main(void)
{
    test();
    return 0;
}

