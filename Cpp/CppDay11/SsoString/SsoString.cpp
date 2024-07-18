#include <iostream>
#include<string.h>
using namespace std;

#define STACK_SIZE 16

class SsoString
{
public:
    class CharProxy
    {
    public:
        CharProxy(SsoString &self, size_t idx);
        char & operator=(const char &ch);
        friend ostream & operator << (ostream &os, const CharProxy &rhs);
    private:
        SsoString & _self;
        size_t _idx;
    };
    friend ostream & operator << (ostream & os, const CharProxy &rhs);
public:
    SsoString()
    {
        cout << "SsoString" <<endl;
        _capacity = STACK_SIZE - 1;
        _size = 0;
    }
    SsoString(const char * pstr)
    {
        cout << "SsoString(const char * pstr)" << endl;
        _size = strlen(pstr);
        if(_size <= STACK_SIZE){
            _capacity = STACK_SIZE - 1;
            strcpy(_buffer._local, pstr);
        }
        else{
            _buffer._pointer = new char[_size + 1 ]();
            _capacity = _size;
            strcpy(_buffer._pointer, pstr); 
        }
    }
    SsoString(const SsoString &rhs)
    {   
        cout << "SsoString(const SsoString &rhs)" <<endl;
        _buffer = rhs._buffer;               
    }
    ~SsoString()
    {
        cout << "~SsoString" << endl;
        if(this->_capacity > STACK_SIZE){
            delete [] _buffer._pointer;
        }
    }
    SsoString & operator=(const SsoString &rhs){
        if(this != &rhs){
            if(rhs._capacity < STACK_SIZE){
                memset(_buffer._local, 0, STACK_SIZE);
                strcpy(_buffer._local, rhs._buffer._local);
                cout << "small copy" <<endl;
            }
            else{
                _buffer._pointer = new char[rhs._capacity +1]; 
                _capacity = rhs._capacity;
                strcpy(_buffer._pointer, rhs._buffer._pointer);
                cout << "big copy" << endl;  
            }
        }
        return *this;
    }
    CharProxy  operator [](size_t idx){
        return CharProxy(*this, idx);
    }
    friend ostream & operator << (ostream &os, const SsoString &rhs);
private:
    union Buffer //存储栈缓冲区的字符串 或者 指向堆的指针
    {
        char * _pointer;
        char _local[16];//
    };

    Buffer _buffer;
    size_t _size;//
    size_t _capacity;//
};

ostream & operator << (ostream &os, const SsoString &rhs){
    if(rhs._capacity < STACK_SIZE){
        os << rhs._buffer._local;
    }else{
        os << rhs._buffer._pointer;
    }
    return os;
}

SsoString::CharProxy::CharProxy(SsoString &self, size_t idx)
:_self(self), _idx(idx)
{
    
}


char & SsoString::CharProxy::operator=(const char &ch){
    if(_self._capacity < STACK_SIZE){
        _self._buffer._local[_idx] = ch;
        return _self._buffer._local[_idx];
    }else{
        _self._buffer._local[_idx] = ch;
        return _self._buffer._pointer[_idx];
    }
}

ostream & operator << (ostream &os, const SsoString::CharProxy &rhs){
    if(rhs._self._capacity < STACK_SIZE){
        os << rhs._self._buffer._local[rhs._idx];    
    }else{
        os << rhs._self._buffer._pointer[rhs._idx];
    }
    return os;
}

int main()
{
    SsoString st1 = "hellohellohello";
    SsoString st2;
    st2 = st1;
    SsoString st3 = "hellohellohellohellohellohello1";
    SsoString st4 = "hellohellohellohellohellohello2";
    st4 = st3;
    st2[0] = 'L';
    cout << st2 << endl;
    return 0;
}

