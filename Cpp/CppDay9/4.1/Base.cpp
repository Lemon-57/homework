#include <iostream>
using namespace std;

class Base
{
public:
    Base(int x)
    :_x(x)
    {
        cout << "Base" << endl;
    }
    void display() const{
        cout << _x << endl;
    }
    int operator +(const Base &rhs){
        return int(rhs._x - this->_x);
    }

private:
    int _x;
};

int main()
{
    int i=2;int j=7;
    Base x(i);
    x.display();
    Base y(j);
    y.display();
    cout << x + y << endl;
    cout << (x + y == j - i) << endl;
    return 0;
}

