 ///
 /// @file    cowCowString.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-28 10:54:23
 ///
 
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

class CowString
{
public:
	CowString()
	: _pstr(new char[kRefcountCapacity + 1]() + kRefcountCapacity)
	{
		init_use_count();
	}

	CowString(const char * pstr)
	: _pstr(new char[strlen(pstr) + kRefcountCapacity + 1]() + kRefcountCapacity)
	{
		init_use_count();
		strcpy(_pstr, pstr);
	}

	CowString(const CowString & rhs)
	: _pstr(rhs._pstr)
	{
		increase();
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

	char & operator[](size_t idx)
	{	return _pstr[idx];	}

	int use_count() const 
	{	return *reinterpret_cast<int*>(_pstr - kRefcountCapacity);	}

private:
	void init_use_count()
	{	*reinterpret_cast<int*>(_pstr - kRefcountCapacity) = 1;	}

	void increase()
	{	++*reinterpret_cast<int*>(_pstr - kRefcountCapacity);	}

	void decrease()
	{	--*reinterpret_cast<int*>(_pstr - kRefcountCapacity);}


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
	CowString str3 = str1;//深拷贝
	cout << "\n执行拷贝构造之后:" << endl;
	cout << "str3:" << str3 << endl;
	cout << "str3's use_count:" << str3.use_count() << endl;

	cout << "str1:" << str1 << endl;
	cout << "str1's use_count:" << str1.use_count() << endl;

	CowString str4("hello, wangdao");
	cout << "\nstr4:" << str4 << endl;
	cout << "str4's use_count:" << str4.use_count() << endl;
	str4 = str1;//浅拷贝
	cout << "\n执行完赋值操作以后:\nstr4:" << str4 << endl;
	cout << "str4's use_count:" << str4.use_count() << endl;
	cout << "str1:" << str1 << endl;
	cout << "str1's use_count:" << str1.use_count() << endl;
	cout << "str3:" << str3 << endl;
	cout << "str3's use_count:" << str3.use_count() << endl;
	
	cout << "\n执行写操作:" << endl;
	str4[0] = 'X';
 
} 
 
int main(void)
{
	test0();
	return 0;
}
