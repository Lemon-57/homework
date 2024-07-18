#include <stdio.h> //C标准库的头文件都是以.h结尾
#include <iostream> // C++标准库头文件没有后缀
                    //C++标准库是以模板类实现的
using namespace std; //引出了一个新的概念：命名空间，C语言没有
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    //stdout 标准输出
    //cout 是一个对象
    // << 重定向/左移运算符
    // 在C++中与cout结合之后为输出流运算符
    cout << "hello world" << endl;
    return 0;
}