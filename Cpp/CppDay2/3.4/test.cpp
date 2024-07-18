#include <iostream>
using namespace std;
 
class Base1
{
public:
    Base1(void){cnt++;cout<<"Base1::constructor("<<cnt<<")"<<endl;}
    ~Base1(void){cnt--;cout<<"Base1::deconstructor("<<cnt + 1<<")"<<endl;}
private:
    static int cnt;
};
int Base1::cnt = 0;
 
class Base2
{
public:
    Base2(int m){num = m; cout<<"Base2::constructor("<<num<<")"<<endl;}
    ~Base2(void){cout<<"Base2::deconstructor("<<num<<")"<<endl;}
private:
    int num;
};
 
class Example
{
public:
    Example(int n){num = n; cout<<"Example::constructor("<<num<<")"<<endl;}
    ~Example(void){cout<<"Example::deconstructor("<<num<<")"<<endl;}
private:
    int num;
};
 
class Derived:public Base1, public Base2
{
public:
    Derived(int m, int n):Base2(m),ex(n){cnt++;cout<<"Derived::constructor("<<cnt<<")"<<endl;}
    ~Derived(void){cnt--;cout<<"Derived::deconstructor("<<cnt+1<<")"<<endl;}
private:
    Example ex;
    static Example stex;    //Example::constructor(1) //不能输出
    static int cnt;
};
int Derived::cnt = 0;
 
Derived ge_a(1,2); // Base1::constructor(1)
                   // Base2::constructor(1)
                   // Example::constructor(2)
                   // Derived::constructor(1)
static Derived gs_b(3,4);   // Base1::constructor(2)
                            // Base2::constructor(3)
                            // Example::constructor(4)
                            // Derived::constructor(2)
int main(void)
{
    cout << "---------start---------" << endl;
    Derived d(5,6); // Base1::constructor(3)
                    // Base2::constructor(5)
                    // Example::constructor(6)
                    // Derived::constructor(3)
    Derived e(7,8); // Base1::constructor(4)
                    // Base2::constructor(7)
                    // Example::constructor(8)
                    // Derived::constructor(4)
    cout << "----------end----------" << endl;
 
    //Derived e(7,8) 析构
            // Derived::deconstructor(4)
            // Example::deconstructor(8)
            // Base2::deconstructor(7)
            // Base1::deconstructor(4)
 
    //Derived d(5,6) 析构
            // Derived::deconstructor(3)
            // Example::deconstructor(6)
            // Base2::deconstructor(5)
            // Base1::deconstructor(3)
    return 0;
}
//static Derived gs_b(3,4) 析构
        // Derived::deconstructor(2)
        // Example::deconstructor(4)
        // Base2::deconstructor(3)
        // Base1::deconstructor(2)
//Derived ge_a(1,2) 析构
        // Derived::deconstructor(1)
        // Example::deconstructor(2)
        // Base2::deconstructor(1)
        // Base1::deconstructor(1)
 
//static Example stex 析构
        //Example::deconstructor(1) //不能输出
