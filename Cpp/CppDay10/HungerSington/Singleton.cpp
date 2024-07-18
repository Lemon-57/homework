#include <iostream>
#include<pthread.h>
using std::cout;
using std::endl;

class Singleton
{
public:
    static Singleton *getInstance(){
        pthread_once(&_once, init);
        return _pInstance;
    }
    static void init(){
        atexit(Destory);
        _pInstance = new Singleton();
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
    static pthread_once_t _once;
};

Singleton * Singleton::_pInstance = nullptr;
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;

int main()
{
    Singleton * p1 = Singleton::getInstance();
    p1->Init(1 ,2);
    p1->print();
    
    Singleton * p2 = Singleton::getInstance();
    p2->print();
    
    return 0;
}
