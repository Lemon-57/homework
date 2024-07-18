#include<iostream>
#include<cstring>
using std::cout;
using std::endl;

class String
{
public:
	String();
	String(const char *pstr);
	String(const String &rhs);
	String &operator=(const String &rhs);
	~String();
	void print();
    size_t length() const;
    const char * c_str() const;
private:
	char * _pstr;
    size_t lenth;
};

String::String(){
    lenth = 0;
    _pstr = new char[lenth];
    strcpy(_pstr, "");
}
String::String(const char *pstr){
    lenth = strlen(pstr)+1;
    _pstr = new char[lenth +1];
    strcpy(_pstr,pstr);
}
String::String(const String &rhs){
    lenth = strlen(rhs._pstr)+1;
    _pstr = new char[lenth +1];
    strcpy(_pstr,rhs._pstr);
}

String &String::operator=(const String &rhs){
    if(this != &rhs)
    {
        delete []_pstr;
        _pstr = nullptr;
        _pstr = new char[strlen(rhs._pstr+1)];
        strcpy(_pstr, rhs._pstr);
    }
    return *this;
}

String::~String(){
    delete []_pstr;
    _pstr = nullptr;
}

void String::print(){
    cout<<_pstr<<endl;
}

size_t String::length() const{
    return strlen(this->_pstr);
}

const char * String::c_str() const{
    return this->_pstr;
}

int main()
{
	String str1;
	str1.print();
	

	String str2 = "Hello,world";
	String str3("wangdao");
	
	str2.print();		
	str3.print();	
	
	String str4 = str3;
	str4.print();
	
	str4 = str2;
	str4.print();
	
	return 0;
}

