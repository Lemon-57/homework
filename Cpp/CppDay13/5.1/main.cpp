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



int main()
{
    Circle circ(5);
    circ.show();
    return 0;
}

