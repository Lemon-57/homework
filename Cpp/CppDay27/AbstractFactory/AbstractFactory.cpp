#include <iostream>
#include<memory>
using std::endl;
using std::cout;
using std::unique_ptr;

class AbstractProductA{
public:
    virtual void Show() = 0;
    virtual  ~AbstractProductA(){
    };
};

class ProductA1
:public AbstractProductA
{
public:
    ProductA1(){
        cout << "ProductA1()" << endl;
    }
    void Show() override{
        cout <<"void ProductA1::Show()" << endl;
    }
    ~ProductA1(){
        cout << "~ProductA1()" << endl;
    }
};

class ProductA2
:public AbstractProductA
{
public:
    ProductA2(){
        cout << "ProductA2()" << endl;
    }
    void Show() override{
        cout <<"void ProductA2::Show()" << endl;
    }
    ~ProductA2(){
        cout << "~ProductA2()" << endl;
    }
};

class AbstractProductB{
public:
    virtual void Show() = 0;
    virtual  ~AbstractProductB(){
    };
};


class ProductB1
:public AbstractProductB
{
public:
    ProductB1(){
        cout << "ProductB1()" << endl;
    }
    void Show() override{
        cout <<"void ProductB1::Show()" << endl;
    }
    ~ProductB1(){
        cout << "~ProductB1()" << endl;
    }
};

class ProductB2
:public AbstractProductB
{
public:
    ProductB2(){
        cout << "ProductB2()" << endl;
    }
    void Show() override{
        cout <<"void ProductB2::Show()" << endl;
    }
    ~ProductB2(){
        cout << "~ProductB2()" << endl;
    }
};



class AbstractFactory
{
public:
    virtual AbstractProductA * createProductA() = 0;
    virtual AbstractProductB * createProductB() = 0;
    virtual ~AbstractFactory(){
    };
};


class ConcreteFactory1
:public AbstractFactory//ConcreteFactory1只生成1号产品
{
public:
    ConcreteFactory1(){
        cout << "ConcreteFactory1()" <<endl;
    }
    AbstractProductA * createProductA() override{
        return new ProductA1();
    }
    AbstractProductB * createProductB() override{
        return new ProductB1();
    }
    ~ConcreteFactory1(){//析构函数重写时不用加 override
        cout << "~ConcreteFactory1()" <<endl;
    }
};

class ConcreteFactory2
:public AbstractFactory//ConcreteFactory2只生成2号产品
{
public:
    ConcreteFactory2(){
        cout << "ConcreteFactory2()" <<endl;
    }
    AbstractProductA * createProductA() override{
        return new ProductA2();
    }
    AbstractProductB * createProductB() override{
        return new ProductB2();
    }
    ~ConcreteFactory2(){//析构函数重写时不用加 override
        cout << "~ConcreteFactory2()" <<endl;
    }
};



void test(){
    AbstractFactory * factory1 = new ConcreteFactory1();//factory1是AbstractFactory类型的基类指针指向了ConcreteFactory1类型的派生类对象
    AbstractProductA * productA1 = factory1->createProductA();//ProductA1是AbstractFactoryA类型的基类指针指向了ProductA1()类型的派生类对象
    productA1->Show();
    AbstractProductB * productB1 = factory1->createProductB();
    productB1->Show();


    AbstractFactory * factory2 = new ConcreteFactory2();
    AbstractProductA * productA2 = factory2->createProductA();
    productA1->Show();
    AbstractProductB * productB2 = factory2->createProductB();
    productB1->Show();
}

int main()
{
    test();
    return 0;
}

