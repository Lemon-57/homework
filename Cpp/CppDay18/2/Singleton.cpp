#include <iostream>
using namespace std;

template <typename T> class Singleton;
class Point{
public:
    Point(int x, int y)
    :_x(x)
    ,_y(y)
    {
        cout << "Point" << endl;
    }
    ~Point(){
        cout << "~Point" << endl;
    }
    void print(){
        cout << _x <<"----"<< _y <<endl;
    }
private:
    int _x;
    int _y;
};

class Computer{
public:
    Computer(string brand, int price)
    :_brand(brand)
    ,_price(price)
    {
        puts("Computer");   
    }
    ~Computer(){
        puts("~Computer");
    }
    void print(){
        cout << _brand <<"----"<< _price <<endl;
    }

private:
    string _brand;
    int _price;
};

template <typename T>
class Singleton{
public:
    template<typename...Args>
    static T * getInstance(Args...args){//将传进的参数打包
        if(nullptr == _pIncetance){
            _pIncetance = new T(args...);//将传进的参数解包
            _ar;//为了在模板参数推导时创建时创建ar对象
            return _pIncetance;
        }
        return _pIncetance;
    }
    class AutoRelease{
        public:
            AutoRelease(){
                puts("AutoRelease");
            }  
            ~AutoRelease(){
                puts("~AutoRelease");
                if(_pIncetance)
                {
                    delete _pIncetance;
                    _pIncetance = nullptr;
                }
            }
    };
private:
    Singleton(){
        cout << "Singleton" << endl;
    }
    ~Singleton(){
        cout << "~Singleton"<<endl;
    }
private:
    static T * _pIncetance;
    static AutoRelease _ar;
    T classt;
};

template<typename T>
T * Singleton<T>::_pIncetance = nullptr;

//Singleton::AutoRelease _ar; //之前的这里是全局变量

//前面的typename表明这里是一个类型 现在这里是全局变量模板，模板需要实例化
template<typename T>
typename Singleton<T>::AutoRelease  Singleton<T>::_ar;


int main()
{
    Computer * pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    pc1->print();
    Computer * pc2 = Singleton<Computer>::getInstance("Huawei", 7777);
    pc2->print();
    Point * pt3 = Singleton<Point>::getInstance(1, 3);
    pt3->print();
    Point * pt4 = Singleton<Point>::getInstance(1, 4);
    pt4->print();

    return 0;
}

