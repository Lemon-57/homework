#include <iostream>
#include<functional>
#include<math.h>
using std::function;
using std::cout;
using std::endl;
using std::bind;

//抽象类作为接口使用的例子
//可以使用多态
//
//面向对象的设计原则：开闭原则
//std::bind + std::function 实现多态的时候，没有虚函数(基于对象)
//(使用形式上就更加灵活)
//
//继承+虚函数，可以实现多态(面向对象思想)
//派生类必须重新定义基类中的虚函数(派生类与基类中虚函数的名字、参数列表、返回类型)
class Figure
{
public:
    //将返回值为void的函数容器function 重命名为DiaplayCallback
    using DisplayCallback = function<void()>;
    using AreaCallback = function<double()>;

    DisplayCallback _diaplayCallback; //定义函数对象实例
    AreaCallback _areaCallback;
    //注册回调函数
    void SetDisplayCallback(DisplayCallback &&cb)
    {
        _diaplayCallback = std::move(cb);
    }
    void SetAreaCallback(AreaCallback &&cb)
    {
        _areaCallback = std::move(cb);
    }
    //执行回调函数
    void handleDisplayCallback() const{
        if(_diaplayCallback){
            _diaplayCallback();//对象实例调用包装的实体
        }
    }
    double handleAreaCallback() const{
        if(_areaCallback){
            return _areaCallback();//对象实例调用包装的实体
        }else{
            return 0;
        }
    }    
};
//执行回调函数
void test(const Figure &fig){//const对象调用const成员函数
    fig.handleDisplayCallback();
    cout << "s area is :" << fig.handleAreaCallback() <<endl;
}

class Rectangle
{
public:
    Rectangle(double length, double width)
    :_length(length)
    ,_width(width)
    {
    }
    void display(int x)const
    {
        cout << "Reactangle"<<endl;
    }
    double area()const{
        return _length * _width;
    }
private:
    double _length;
    double _width;
};

class Circle
{
public:
    Circle(double radis)
    :_radis(radis)
    {
    }
    void show()const
    {
        cout << "Circle"<<endl;
    }
    double getArea()const{
        return _radis * _radis * 3.14;
    }
private:
    double _radis;
};

class Traingle
{
public:
    Traingle(double a, double b, double c)
    :_a(a)
    ,_b(b)
    ,_c(c)
    {
    }
    void print()const
    {
        cout << "Traingle"<<endl;
    }

    double calcArea()const{
        double tmp = (_a + _b + _c)/2;
        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp - _c));;
    }
private:
    double _a;
    double _b;
    double _c;
};

void test1(){
    Rectangle rectangle(10, 20); 
    Circle circle(10); 
    Traingle traingle(3, 4, 5); 
    Figure figure; 
    figure.SetDisplayCallback(bind(&Rectangle::display, &rectangle, 10));
    figure.SetAreaCallback(bind(&Rectangle::area, &rectangle)); 
    test(figure); 
    cout<<"------"<<endl;
    figure.SetDisplayCallback(bind(&Circle::show, &circle)); 
    figure.SetAreaCallback(bind(&Circle::getArea, &circle)); 
    test(figure); 
    cout<<"------"<<endl;
    figure.SetDisplayCallback(bind(&Traingle::print, &traingle)); 
    figure.SetAreaCallback(bind(&Traingle::calcArea, &traingle)); 
    test(figure);
}

int main()
{
    test1();
    return 0;
}

