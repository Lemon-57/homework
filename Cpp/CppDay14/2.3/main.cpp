#include <iostream>

using std::endl;
using std::cout;

class Base
{
public:
	Base(int i) 
	{
		b = i;
	}
//　　(1)
    virtual
    void Print(){
        cout<<"Base Print() called."<<endl;
    }
protected:
	int b;
};

class Derive1
:public Base
{
public:
//　　(2)
    Derive1(int base)
    :Base(base)
    {

    }
    void Print()
	{
		cout<<"Derive1's Print() called."<<endl;
	}
};

class Derive2
:public Base
{
public:
    Derive2(int base)
    :Base(base)
    {

    }
    void Print()
	{
		cout<<"Derive2's Print() called."<<endl;
	}
};

void fun(/*(4)*/ Base * obj)
{
    obj->Print();
}

int main(void)
{ 
//	(5)
    Derive1 dderive1(1);
    Derive2 dderive2(2);
    Base bs(1);
    Base * d1 = &dderive1 ;
    Base * d2 = &dderive2;
    fun(d1);
    fun(d2);
	

	return 0;

}
