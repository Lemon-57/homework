#include <iostream>
using std::cout;
using std::endl;

class Singleton
{
public:
    static Singleton *getInstance(){
        if(nullptr == _pInstance){
            _pInstance = new Singleton();
            return _pInstance;
        }
        return _pInstance;
    }
    static void Destory(){
        if(_pInstance){
            delete _pInstance;
            _pInstance = nullptr; 
        }
    }
    void Init(int x, int y)
    {
        _x = x;
        _y = y;
    }
    void print(){
        cout << "x:" << _x << "\ny:" << _y <<endl;
    }
private:
    Singleton(){
        cout << "Singleton" << endl;
    }
    ~Singleton(){
        cout << "~Singleton" << endl;
    }
    static Singleton * _pInstance;
    int _x;
    int _y;
};

Singleton * Singleton::_pInstance = nullptr;

int main()
{
    Singleton * p1 = Singleton::getInstance();
    p1->Init(1 ,2);
    p1->print();
    
    Singleton * p2 = Singleton::getInstance();
    p2->print();
    
    p1->Destory();
    return 0;
}
