#include <iostream>
#include<functional>

using std::cout;
using std::endl;
using std::bind;
using std::function;

int add(int x, int y)
{
    cout << " int add(int, int)" << endl;
    return x + y;
}

int add2(int x, int y, int z)
{
    cout << " int add(int, int)" << endl;
    return x + y + z;
}

class Example
{
public:
    int add(int x, int y)
    {
        cout << " int Example::add(int, int)" << endl;
        return x + y;
    }
    static int func3(int x, int y)
    {
        cout << " int Example::func3(int, int)" << endl;
        return x + y;
    }
    int data = 12345;//data是数据成员
};


void test(){
    //bind可以改变函数的形态(类型)
    //函数的类型由函数的返回值类型与函数的参数列表进行确定
    auto f1 = bind(add, 1, 3);
    cout << "f1() = " << f1() << endl;
    auto f2 = bind(add2, 1, 2, 3);
    cout << "f2() = " << f2() << endl;
    //bind可以绑定到普通函数，可以绑定到成员函数
    //no-static成员函数需要传递this指针
    //bind绑定成员函数的时候，要考虑this指针的影响，
    //此处可以使用值传递(会执行拷贝操作，效率有损失)
    //也可以使用地址传递(对象此时应该还必须存在否则就是空指针)
    Example ex;
    auto f3 = bind(&Example::add, &ex, 10, 20);//地址传递
    cout << "f3() = " << f3() << endl;
    //static成员函数不需要传this指针
    auto f4 = bind(&Example::func3, 10, 20);//值传递
    cout << "f4() = " << f4() << endl;
}

int func4(){
    return 5;
}

int func5(){
    return 10;
}
//pFunc1是一个函数指针， 可以指向int (void) *
int (*pFunc1)();

//pFunc就是一个函数指针 类型
typedef int (*pFunc2)();

void test2(){
    //在C语言中函数名就是函数的入口地址
    //数组名就是数组的入口地址
    //注册 执行分开(延迟函数的执行)
    pFunc1 = func4;
    pFunc2 f2 = &func4; //注册回调函数  //func4是一个
    pFunc2 f3 = func4; //注册回调函数
    cout << "f2() = " << f2() << endl;//执行回调函数
    f2 = func5; //重新绑定
    cout << "f2() = " << f2() << endl;
}




void func6(int x1, int x2, int x3, const int &x4, int x5)
{
    cout << "x1 = " << x1 << endl
        << "x2 = " << x2 << endl
        << "x3 = " << x3 << endl
        << "x4 = " << x4 << endl
        << "x5 = " << x5 << endl;
}

void test3(){
    using namespace std::placeholders;
    int number = 10;
    //占位符本身占据的是形参的位置
    //占位符中的数字代表的是实参的位置
    //bind默认情况采用的是值传递
    auto f1 = bind(func6, 1, _3, _10, number, number);
    //std::cref = const reference(引用的包装器)
    //auto f2 = bind(func6, 1, _3, _10, std::cref(number), number);
    auto f2 = bind(func6, 1, _3, _10, std::cref(number), number);
    number = 333;
    //必须要有10个数据，因为不能直接找到_3 和 _10
    f1(11, 22, 33, 44, 55, 66, 77, 88, 99, 100);//没有使用的实参直接抛弃
    cout<<"-------"<<endl;
    f2(11, 22, 33, 44, 55, 66, 77, 88, 99, 100);//没有使用的实参直接抛弃

}

void test4()
{
    //std::bind +std::function进行实现多态
    //
    //bind可以改变函数的形态(类型)
    //int(int, int)--->int()
    //function可以用来装函数类型，function称为函数的容器
    function<int()> f = bind(&add, 1, 3);
    cout << "f() = " << f() << endl;

    Example ex;
    //int(this, int, int)--->int()
    function<int()> f2 = bind(&Example::add, &ex, 10, 20);
    cout << "f2() = " << f2() << endl;

    using namespace std::placeholders;
    //int(int, int)---->int(int)
    function<int(int)> f4 = bind(&add, 1, _1);
    cout << "f4(100) = " << f4(100) << endl;

    //可以绑定到数据成员上面
    /* f2 = bind(&Example::data, &ex); */
    function<int()> f3 = bind(&Example::data, &ex);
    cout << "f3() = " << f3() << endl;
}

void test5(){
    cout << test5 <<endl;
    cout << *test5 << endl;
    cout << &test5 <<endl;
    cout << (void*)test5 <<endl;
    cout << (void*)*test5 << endl;
    cout << (void*)&test5 <<endl;
    //%p输出的长度是一致的8位16进制符（即32位2进制符）
    printf("%p\n", test5);
    printf("%p\n", *test5);
    printf("%p\n", &test5);     
}


int main()
{
    // test();
    test2();
    // test3();
    // test4();
    // test5();
    return 0;
}

