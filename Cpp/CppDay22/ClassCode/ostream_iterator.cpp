#include <iostream>
#include<iterator>
#include<vector>
using namespace std;



void test(){
    vector<int> number = {1, 3, 5, 7, 9, 8};
    //流迭代器可以看成容器
    ostream_iterator<int>osi(cout, "\n");//
    //在copy中进行一个循环
    copy(number.begin(), number.end(), osi);
    cout << 123;
    cout.operator<<(123);
} 

int main()
{
    test();
    int *pInt = new int(10);
    const char *pstr = "hello";
    // cout << pstr << endl;
    // cout << *pstr << endl;//打印的是字符串的第一个字符
    // cout << *pInt << endl;
    return 0;
}

