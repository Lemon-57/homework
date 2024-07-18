#include <iostream>
#include<memory>
using std::endl;
using std::cout;
using std::unique_ptr;

enum Productype{
    TypeA,
    TypeB,
    TypeC
};

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

class ProductC
:public Product
{
public:
    ProductC(){
        cout << "ProductC()" << endl;
    }
    void Show() override{
        cout <<"void ProductC::Show()" << endl;
    }
    ~ProductC(){
        cout << "~ProductC()" << endl;
    }
};

class ProductFactory
{
public:
    static unique_ptr<Product> CreateProduct (Productype type){//返回值类型是智能指针
        switch (type)
        {
        case TypeA:
            return unique_ptr<Product>(new ProductB());//基类的指针指向派生类的对象 （但是可操作的空间只有基类的空间） 多态被激活
            break;
        case TypeB:
            return unique_ptr<Product>(new ProductB());//基类的指针指向派生类的对象 （但是可操作的空间只有基类的空间） 多态被激活
            break;     
        case TypeC:
            return unique_ptr<Product>(new ProductC());//基类的指针指向派生类的对象 （但是可操作的空间只有基类的空间） 多态被激活
            break;        
        default:
            return unique_ptr<Product>(nullptr);
            break;
        }
    }

};

void test(){
    unique_ptr<Product>pa = ProductFactory::CreateProduct(TypeA);
    unique_ptr<Product>pb = ProductFactory::CreateProduct(TypeB);
    unique_ptr<Product>pc = ProductFactory::CreateProduct(TypeC);
    pa->Show();
    pb->Show();
    pc->Show();
}

int main()
{
    test();
    return 0;
}

