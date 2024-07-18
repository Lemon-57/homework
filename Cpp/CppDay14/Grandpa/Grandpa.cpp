#include <iostream>
using namespace std;

class Grandpa
{ 
public:
    Grandpa()
    {
    cout << "Grandpa()" << endl;
    } 
    ~Grandpa()
    {
    cout << "~Grandpa()" << endl;
    } 
    
    virtual
    void func1()
    {
    cout << "Grandpa::func1()" << endl;
    } 
    
    virtual
    void func2()
    {
    cout << "Grandpa::func2()" << endl;
    }
};

class Father
: public Grandpa
{ 
    public:
    Father()
    {
        cout << "Father()" << endl;
        func1();
    } 
    ~Father()
    {
        cout << "~Father()" << endl;
        func2();
    } 
    virtual
    void func1()
    {
    cout << "Father::func1()" << endl;
    } 
    virtual
    void func2()
    {
    cout << "Father::func2()" << endl;
    }
};

class Son
:public Father
{ 
public:
    Son()
    {
    cout << "Son()" << endl;
    } ~
    Son()
    {
    cout << "~Son()" << endl;
    } 
    virtual void func1()
    {
    cout << "Son::func1()" << endl;
    } 
    virtual void func2()
    {
    cout << "Son::func2()" << endl;
    }
};

int main()
{
    Son son;
    return 0;
}

