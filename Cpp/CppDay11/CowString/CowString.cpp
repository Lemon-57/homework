#include <iostream>
#include <string.h>
using namespace std;
class CowString
{
public:
    class CharProxy
    {
    public:
        CharProxy(CowString &self, size_t idx);
        char &operator=(const char &ch);
        // 将自定义类型转换为char型
        operator char()
        {
            return _self._pstr[_idx];
        }

    private:
        CowString &_self;
        size_t _idx;
    };

public:
    CowString()
        : _pstr(calloc())
    {
        cout << "CowString()" << endl;
        init_use_count();
    }
    CowString(const char *pstr)
        : _pstr(calloc(pstr))
    {
        cout << "CowString(const char * pstr)" << endl;
        init_use_count();
        strcpy(_pstr, pstr);
    }
    CowString(const CowString &rhs)
        : _pstr(rhs._pstr)
    {
        cout << "CowString(const CowString &rhs)" << endl;
        increase();
    }
    ~CowString()
    {
        cout << "~CowString()" << endl;
        release();
    }
    void release()
    {
        decrease();
        if (0 == use_count())
        {
            delete[] (_pstr - referenceCount);
            cout << "free heap space" << endl;
        }
    }
    CowString &operator=(const CowString &rhs)
    { // 浅拷贝
        if (this != &rhs)
        {
            release();
            _pstr = rhs._pstr;
            increase();
        }
        return *this;
    }

    CharProxy operator[](size_t idx)
    {
        return CharProxy(*this, idx);
    }

    size_t length() const
    {
        return strlen(_pstr);
    }
    int use_count() const
    {
        return *reinterpret_cast<int *>(_pstr - referenceCount);
    }

private:
    void init_use_count()
    {
        *reinterpret_cast<int *>(_pstr - referenceCount) = 1;
    }
    void increase()
    {
        ++*reinterpret_cast<int *>(_pstr - referenceCount);
    }
    void decrease()
    {
        --*reinterpret_cast<int *>(_pstr - referenceCount);
    }
    char *calloc(const char *_pstr = nullptr)
    {
        if (_pstr)
        {
            return new char[strlen(_pstr) + referenceCount + 1]() + referenceCount;
        }
        else
        {
            return new char[referenceCount + 1]() + referenceCount;
        }
    }
    friend ostream &operator<<(ostream &os, const CowString &rhs);

private:
    char *_pstr;
    static const int referenceCount = 4;
};

ostream &operator<<(ostream &os, const CowString &rhs)
{
    os << rhs._pstr;
    return os;
}

CowString::CharProxy::CharProxy(CowString &self, size_t idx)
    : _self(self), _idx(idx)
{
}

char &CowString::CharProxy::operator=(const char &ch)
{
    if (_idx < _self.length())
    {
        if (_self.use_count() > 1)
        {
            char *ptmp = _self.calloc(_self._pstr);
            strcpy(ptmp, _self._pstr);
            _self._pstr = ptmp;
            _self.init_use_count();
        }
        _self._pstr[_idx] = ch;
        return _self._pstr[_idx];
    }
    else
    {
        static char nullchar = '\0';
        return nullchar;
    }
}

int main()
{
    CowString s1 = "hello,world";
    cout << s1.use_count() << endl;
    CowString s2 = s1;
    cout << s2.use_count() << endl;
    s2.release();
    cout << s2.use_count() << endl;
    s1[0] = 'L';
    cout << s1 << endl;
}
