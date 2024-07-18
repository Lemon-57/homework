#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using std::cout;
using std::endl;
using std::vector;
using std::for_each;

class Number
{
public:
    Number(size_t data = 0)
    : _data(data)
    {

    }

    void print() const
    {
        cout << _data << "  ";
    }

    //判断是不是偶数
    bool isEven() const
    {
        return (0 == _data%2);
    }

    //质数
    bool isPrimer() const
    {
        if(1 == _data)
        {
            return false;
        }

        for(size_t idx = 2; idx != _data/2; ++idx)
        {
            if(0 == _data % idx)
            {
                return false;
            }
        }

        return true;
    }
private:
    size_t _data;
};

void test()
{
    vector<Number> vec;
    for(size_t idx = 0; idx != 30; ++idx)
    {
        vec.push_back(Number(idx));
    }

    //成员函数是一个受限制的函数
    for_each(vec.begin(), vec.end(), std::mem_fn(&Number::print));
    /* for_each(vec.begin(), vec.end(), (&Number::print)); */
    cout << endl;

    cout << endl << endl;
    vec.erase(remove_if(vec.begin(), vec.end(), 
                        std::mem_fn(&Number::isEven)), vec.end());
    for_each(vec.begin(), vec.end(), std::mem_fn(&Number::print));
    cout << endl;

    cout << endl << endl;
    vec.erase(remove_if(vec.begin(), vec.end(), 
                        std::mem_fn(&Number::isPrimer)), vec.end());
    for_each(vec.begin(), vec.end(), std::mem_fn(&Number::print));
    cout << endl;
}

int main(int argc, char **argv)
{
    test();
    return 0;
}

