#include <iostream>
using namespace std;
class Data
{
public:
    int getData() const{
        return _data;
    }
private:
    int _data;
};

class MiddleLayer
{
public:
    MiddleLayer(Data *pdata)
    :_pdata(pdata)
    {
        cout << "MiddleLayer" << endl;
    }
    Data *operator->()
    {
        return _pdata;
    }
    Data &operator*()
    {
        return *_pdata;
    }
private:
    Data *_pdata;
};

class ThirdLayer
{
public:
    ThirdLayer(MiddleLayer *m1)
    :_m1(m1)
    {
        cout << "ThirdLayer" << endl;
    }
    MiddleLayer & operator->()
    {
        return *_m1;
    }
    ~ThirdLayer()
    {
        delete _m1;
        cout << "~ThirdLayer" << endl;
    }

private:
    MiddleLayer *_m1;
};

int main()
{
    MiddleLayer ml(new Data());
    cout << ml->getData()<<endl;
    //上下两句等价
    cout << (ml.operator->())->getData() << endl;

    cout << (*ml).getData() << endl;

    ThirdLayer tl(new MiddleLayer(new Data()));

    cout << tl->getData() << endl;
    //上下两句等价
    cout << (tl.operator->()).operator->()->getData() << endl;
    return 0;
}

