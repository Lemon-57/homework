#include <iostream>
#include<iterator>
#include<vector>
#include<algorithm>
using namespace std;


void test(){
    vector<int>vec;
    istream_iterator<int>isi(std::cin);
    // copy(isi, istream_iterator<int>(), vec.begin());
    //vector插入数据使用push_back
    //back_inserter是一个函数模板，返回类型是一个类
    //back_inserter_iterator，该类赋值运算符调用push_back
    copy(isi, istream_iterator<int>(), std::back_inserter(vec));
    //ostream_iterator<int>(cout, " ")无参构造函数的临时对象
    //将vec遍历
    copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
    cout<<endl;
}


int main()
{
    test();
    return 0;
}

