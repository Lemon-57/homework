#include <iostream>
#include<string.h>

using std::cout;
using std::cin;
using std::endl;
class String
{
public:
    String()
    :_pstr(0)
    {

    }
    String(const char *pstr)
    :_pstr(new char[strlen(pstr + 1)])
    {
        cout << "1"<< endl;
        strcpy(_pstr, pstr);
    }

    String(String &&rhs)
    :_pstr(rhs._pstr)
    {
        cout << "2" << endl;
        rhs._pstr = nullptr;
    }
    
    String &operator=(String &&rhs)
    {
        cout << "String &operator =" <<endl;
        if(this != &rhs)
        {
            delete [] _pstr;
            _pstr = nullptr;    
            _pstr = rhs._pstr;
            rhs._pstr = nullptr;
        }
        return *this;
    }
    void print()
    {
        cout << this->_pstr << endl;
    }
    ~String()
    {
        delete []_pstr;
    }
private:
    char *_pstr;
};


int main()
{
    String s1("hello");
    String s2(String("hello"));
    String s3 = "hello";
    String s4;
    s4 = "hello";
    s3.print();
    return 0;
}

