#include <iostream>
#include<memory>
using std::endl;
using std::cout;
using std::unique_ptr;

class Product{
public:
    virtual void Show() = 0;
    virtual  ~Product(){
    };
};

class ProductA
:public Product
{
public:
    ProductA(){
        cout << "ProductA()" << endl;
    }
    void Show() override{
        cout <<"void ProductA::Show()" << endl;
    }
    ~ProductA(){
        cout << "~ProductA()" << endl;
    }
};

class ProductB
:public Product
{
public:
    ProductB(){
        cout << "ProductB()" << endl;
    }
    void Show() override{
        cout <<"void ProductB::Show()" << endl;
    }
    ~ProductB(){
        cout << "~ProductB()" << endl;
    }
};


class Factory
{
public:
    virtual Product * createProduct() = 0;
    virtual ~Factory(){

    };
};


class FactoryA
:public Factory
{
public:
    FactoryA(){
        cout << "FactoryA()" <<endl;
    }
    Product  * createProduct () override{
        return new ProductA();
    }
    ~FactoryA(){//析构函数重写时不用加 override
        cout << "~FactoryA()" <<endl;
    }
};

class FactoryB
:public Factory
{
public:
    FactoryB(){
        cout << "FactoryB()" <<endl;
    }
    Product  * createProduct () override{
        return new ProductB();
    }
    ~FactoryB(){//析构函数重写时不用加 override
        cout << "~FactoryB()" <<endl;
    }
};

void test(){
    unique_ptr<Factory>fa(new FactoryA());//
    unique_ptr<Product>pa (fa->createProduct());
    pa->Show();

    unique_ptr<Factory>fb(new FactoryB());
    unique_ptr<Product>pb (fb->createProduct());
    pb->Show();

}

int main()
{
    test();
    return 0;
}

