#include<iostream>
using std::cout;
using std::endl;

class Singleton
{
public:
    static Singleton *getInstance()
    {
        if(nullptr == _pInstance)
        {
            _pInstance = new Singleton();
        }
        return _pInstance;
    }
private:
    Singleton()
    {
        cout << "Singleton()"<<endl;
    }
    static Singleton *_pInstance;
};

Singleton *Singleton::_pInstance = nullptr;
