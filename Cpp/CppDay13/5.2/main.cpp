#include <iostream>
using namespace std;

#define pi 3.14

class Circle{
public:
    Circle()
    :_r(0)
    {
        
    }
    Circle(double r)
    :_r(r)
    {
        
    }
    double gerArea(){
        return _r * _r * pi;
    }
    double gerPerimeter(){
        return _r * pi * 2;
    }
    void show(){
        cout << "r  " << _r << endl;
        cout << "Area   " << _r * _r *pi <<endl;
        cout << "Perimeter  " << _r * pi * 2 <<endl;
        
    }
private:
    double _r;
};

class Cylinder
:public Circle
{
public:
    Cylinder(double r, double h)
    :Circle(r)
    , _r(r)
    , _h(h)
    {
        
    }
    double getVolume(){
        return Circle::gerArea() * _h;
    }
    void showVolume(){
        cout << "Volume" << Circle::gerArea() *_h << endl;
    }
private:
    double _r;
    double _h;
};


int main()
{
    Cylinder cylind(1, 2);
    cylind.showVolume();
    return 0;
}

