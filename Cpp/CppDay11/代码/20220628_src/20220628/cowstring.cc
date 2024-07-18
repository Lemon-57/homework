 ///
 /// @file    cowCowString.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-28 10:54:23
 ///
 
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;


class CowString
{
public:
	class CharProxy
	{
	public:
		//1. 对于外部类的静态成员，在内部类的成员函数中
		//可以直接拿到,不需要传参
		//2. 对于外部类的非静态成员，要访问，首先就要传参
		CharProxy(CowString & self, size_t idx);
		char & operator=(const char & ch);
		
		friend ostream & operator<<(ostream & os, const CharProxy & rhs);
	
	private:
		CowString & _self;//要操作就是字符串，因此要保存一个引用，而不是对象
		size_t _idx;
	};
	//既是CowString的友元,又是CharProxy的友元
	friend ostream & operator<<(ostream & os, const CowString::CharProxy & rhs);
public:
	CowString()
	: _pstr(calloc())
	{
		init_use_count();
	}

	CowString(const char * pstr)
	: _pstr(calloc(pstr))
	{
		init_use_count();
		strcpy(_pstr, pstr);
	}

	CowString(const CowString & rhs)
	: _pstr(rhs._pstr) //浅拷贝
	{
		increase();//引用计数加1
	}

	~CowString() {	release();}

	void release()
	{
		//1. 先将引用计数减1
		decrease();
		//2. 当引用计数为0时，才真正回收空间
		if(use_count() == 0) {
			delete [] (_pstr - kRefcountCapacity);
			cout << ">> free heap space" << endl;
		}
	}

	CowString & operator=(const CowString & rhs) 
	{
		if(this != &rhs) {//1. 自复制
			release();	  //2. 回收左操作数空间
			_pstr = rhs._pstr;//3. 浅拷贝
			increase();	  //4. 引用计数加1
		}	
		return *this;// 5. 返回
	}

	CharProxy operator[](size_t idx)
	{	return CharProxy(*this, idx);	}

#if 0
	//下标访问运算符
	//bug: 该运算符无法确定接下来要做的读操作还是写操作
	char & operator[](size_t idx)
	{	
		//如果发现其引用计数大于1的情况下，需要进行深拷贝
		if(idx < length()) {
			if(use_count() > 1) {
				decrease();
				char * ptmp = calloc(_pstr);//深拷贝
				strcpy(ptmp, _pstr);
				_pstr = ptmp;
				init_use_count();
			}
			return _pstr[idx];
		} else {
			static char nullchar = '\0';
			return nullchar;
		}
	}
#endif

	size_t length() const {	return strlen(_pstr);	}

	int use_count() const 
	{	return *reinterpret_cast<int*>(_pstr - kRefcountCapacity);	}

private:
	void init_use_count()
	{	*reinterpret_cast<int*>(_pstr - kRefcountCapacity) = 1;	}

	void increase()
	{	++*reinterpret_cast<int*>(_pstr - kRefcountCapacity);	}

	void decrease()
	{	--*reinterpret_cast<int*>(_pstr - kRefcountCapacity);}

	char * calloc(const char * pstr = nullptr)
	{	
		if(pstr) {
			return new char[strlen(pstr) + kRefcountCapacity + 1]() + kRefcountCapacity;
		} else {
			return new char[kRefcountCapacity + 1]() + kRefcountCapacity;
		}
	}

	friend std::ostream & operator<<(std::ostream & os, const CowString & rhs);

private:
	char * _pstr;
	static const int kRefcountCapacity = 4;
};

std::ostream & operator<<(std::ostream & os, const CowString & rhs)
{
	os << rhs._pstr;
	return os;
}

CowString::CharProxy::CharProxy(CowString & self, size_t idx)
: _self(self)
, _idx(idx)
{
	//非静态成员只与对象在一起，因此必须要有对象
}
		
//写操作的实现
char & CowString::CharProxy::operator=(const char & ch)
{
	if(_idx < _self.length()) {
		if(_self.use_count() > 1) {
			_self.decrease();
			char * ptmp = _self.calloc(_self._pstr);//深拷贝
			strcpy(ptmp, _self._pstr);
			_self._pstr = ptmp;
			_self.init_use_count();
		}
		_self._pstr[_idx] = ch;//执行写操作
		return _self._pstr[_idx];
	} else {
		static char nullchar = '\0';
		return nullchar;
	}
}
		
//读操作的实现
ostream & operator<<(ostream & os, const CowString::CharProxy & rhs)
{
	os << rhs._self._pstr[rhs._idx];
	return os;
}
 
void test0() 
{
	CowString str0;
	cout << "str0's use_count:" << str0.use_count() << endl;
	cout << "str0:" << str0 << endl;

	CowString str1("hello,world");//各自创建的字符串
	CowString str2("hello,world");//没有深拷贝
	cout << "str1:" << str1 << endl;
	cout << "str1's use_count:" << str1.use_count() << endl;

	cout << "str2:" << str2 << endl;
	cout << "str2's use_count:" << str2.use_count() << endl;

	//需要优化的是以下这两个行为
	CowString str3 = str1;//浅拷贝
	cout << "\n执行拷贝构造之后:" << endl;
	cout << "str3:" << str3 << endl;
	cout << "str3's use_count:" << str3.use_count() << endl;

	cout << "str1:" << str1 << endl;
	cout << "str1's use_count:" << str1.use_count() << endl;

	CowString str4("hello, wangdao");
	CowString str5 = str4;
	cout << "\nstr5:" << str5 << endl;
	cout << "str5's use_count:" << str5.use_count() << endl;
	cout << "\nstr4:" << str4 << endl;
	cout << "str4's use_count:" << str4.use_count() << endl;
	str4 = str1;//浅拷贝
	//str4.operator=(str1);
	cout << "\n执行完赋值操作以后:\nstr4:" << str4 << endl;
	cout << "str4's use_count:" << str4.use_count() << endl;
	cout << "str1:" << str1 << endl;
	cout << "str1's use_count:" << str1.use_count() << endl;
	cout << "str3:" << str3 << endl;
	cout << "str3's use_count:" << str3.use_count() << endl;
	cout << "\nstr5:" << str5 << endl;
	cout << "str5's use_count:" << str5.use_count() << endl;
	
	cout << "\n执行写操作:" << endl;
	str4[0] = 'X';//完成了写时复制的操作
	cout << "\nstr4:" << str4 << endl;
	cout << "str4's use_count:" << str4.use_count() << endl;
	cout << "str1:" << str1 << endl;
	cout << "str1's use_count:" << str1.use_count() << endl;
	cout << "str3:" << str3 << endl;
	cout << "str3's use_count:" << str3.use_count() << endl;

	cout << "\n执行读操作:" << endl;
	cout << str1[0] << endl;
	cout << "\nstr1:" << str1 << endl;
	cout << "str1's use_count:" << str1.use_count() << endl;
	cout << "str3:" << str3 << endl;
	cout << "str3's use_count:" << str3.use_count() << endl;
} 
 
int main(void)
{
	test0();
	return 0;
}
