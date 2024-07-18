#include <iostream>

//值的类型
void test1(){
    /* &10;//error 字面值常量，是右值，不能进行取地址 */
    &"hello,world"; //字符串常量，左值，文字常量区
    /* &string("hello,word"); //右值 */
}

void test2(){
    int number = 10;
    int &ref = number;

    const int &ref = 10;
    const int &ref2 = number;
}


int main()
{
    test1();
    std::cout << "Hello world" << std::endl;
    return 0;
}

