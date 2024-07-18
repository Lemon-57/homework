 ///
 /// @file    string.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-06-28 17:14:26
 ///
 
#include <iostream>
using std::cout;
using std::endl;
using std::string;
 
void test0() 
{
	int * pint  = new int(1);
	cout << "pint:" << pint << endl;

	cout << "sizeof(string):" << sizeof(string) << endl;
	string s1("hello,wangdao11");//15个字节,存在栈上
	cout << "sizeof(s1):" << sizeof(s1) << endl;
	printf("&s1: %p\n", &s1);
	printf("address of s1:%p\n", s1.c_str());

	string s2("hello,wangdao111");//16个字节
	cout << "sizeof(s2):" << sizeof(s2) << endl;
	printf("&s2: %p\n", &s1);
	printf("address of s2:%p\n", s2.c_str());

	string s3("hello,wangdao11122222222222222222222222222222222222222222222222");//16个字节
	cout << "sizeof(s3):" << sizeof(s3) << endl;
	printf("address of s3:%p\n", s3.c_str());

	string s4 = s1;//短字符串在栈上进行复制，效率很高
	cout << "\ns4:" << s4 << endl;
	printf("address of s4:%p\n", s4.c_str());

	//(无锁编程)
	//
	//STL中的容器都没有做到多线程安全
	string s5 = s3;//字符串内容进行深拷贝
	cout << "\ns5:" << s5 << endl;
	printf("address of s5:%p\n", s5.c_str());

	delete pint;
} 
 
int main(void)
{
	test0();
	return 0;
}
