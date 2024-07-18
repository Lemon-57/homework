#include<string.h>
#include<stdio.h>
#include"String.hpp"

using namespace std;


String::String()//生成一个空字符串
:_pstr(nullptr)
{
    cout << "String()" << endl; 
}

String::String(const char *pstr)
:_pstr(new char[strlen(pstr) + 1]())
{
    strcpy(_pstr, pstr);
    cout << "String(const char *pstr)" <<endl;
}

String::String(const String &rhs)
:_pstr(new char[strlen(rhs._pstr) + 1]())
{
    cout << "String(const char *pstr)" <<endl;
    strcpy(_pstr, rhs._pstr);
}

String::~String()
{
    if(_pstr){
        delete [] _pstr;
    }
    cout << "~String()" << endl;
}

String& String::operator=(const String &str)
{
    if(*this != str){
        delete [] _pstr;
        _pstr = nullptr;
        _pstr = new char[strlen(str._pstr)+1]();
        strcpy(_pstr,str._pstr);
    }
    return *this;
}

String& String::operator=(const char *str){
    if(strcmp(_pstr, str)){
        delete [] _pstr;
        _pstr = new char[strlen(str)+1]();
        strcpy(_pstr,str);
    }
    return *this;
}
//字符串追加
String & String::operator+=(const String &str){
    char * newStr = new char[this->size() + str.size()+1];
    sprintf(newStr, "%s%s", _pstr, str._pstr);
    delete [] _pstr;
    _pstr = new char[this->size() + str.size()+1];
    strcpy(_pstr, newStr);
    delete [] newStr;
    return *this;
}

String & String::operator+=(const char *str)
{
    char * newStr = new char[this->size() + strlen(str)+1];
    sprintf(newStr, "%s%s", _pstr, str);
    delete [] _pstr;
    _pstr = new char[this->size() + strlen(str)+1];
    strcpy(_pstr, newStr);
    delete [] newStr;
    return *this;
}

char &String::operator[](size_t index){
    return _pstr[index];
}

const char & String::operator[](size_t index) const{
    return _pstr[index];
}

size_t String::size() const{
    return strlen(_pstr);
}

const char* String::c_str() const{
    return _pstr;
}

bool operator==(const String &str1, const String &str2){
    return !strcmp(str1._pstr, str2._pstr);
}

bool operator!=(const String &str1, const String &str2){
    return strcmp(str1._pstr, str2._pstr);
}

bool operator<(const String &str1, const String &str2){
    return strcmp(str1._pstr, str2._pstr) < 0;
}

bool operator>(const String &str1, const String &str2){
    return strcmp(str1._pstr, str2._pstr) > 0;
}

bool operator<=(const String &str1, const String &str2){
    return strcmp(str1._pstr, str2._pstr) <= 0;
}

bool operator>=(const String &str1, const String &str2){
    return strcmp(str1._pstr, str2._pstr) >= 0;

}

std::ostream &operator<<(std::ostream &os, const String &s){
    if(s.size()){
        os << s._pstr;  
    }
    return os;
}

std::istream &operator>>(std::istream &is, String &s){
    if(s.size()){
        is >> s._pstr;
    }
    return is;
}
//重载字符串连接运算，这个运算会返回一个新的字符串
String operator+(const String &str1, const String &str2)
{
    char *_pstr = new char[str1.size() + str2.size()+1];
    sprintf(_pstr, "%s%s", str1.c_str(), str2.c_str());
    return String(_pstr);
}

String operator+(const String &str1, const char *str2){
    char *_pstr = new char[str1.size() + strlen(str2)+1];
    sprintf(_pstr, "%s%s", str1.c_str(), str2);
    return String(_pstr);

}

String operator+(const char *str1, const String &str2){
    char *_pstr = new char[str2.size() + strlen(str1)+1];
    sprintf(_pstr, "%s%s",  str1, str2.c_str());
    return String(_pstr);

}
void String::print(){
    if(_pstr){
        cout << _pstr << endl;
    }
}

int main (){
    //String str();//其实是一个函数声明, 不是创建一个对象
	String str;
	cout << "1111" << endl;
	str.print();
	cout << "222" << endl;

    String str1("hello");
    str1.print();
    cout << endl;

    String str2 = "hello, world";//const char *
    str2.print();
    cout << endl;

    String str3 = str2;//调用拷贝构造函数
    cout << "str3:";
    str3.print();
    cout << endl;

    String str4("hello,wuhan");
    cout << "str4:";
    str4.print();
    cout << endl;
 
    str3 = str2;//调用赋值函数
    cout << "str3:";
    str3.print();
    cout << endl;
    return 0;
}

