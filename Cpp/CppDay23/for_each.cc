#include <iostream>
#include <algorithm>
#include<vector>
#include<iterator>
using namespace std;

void print(int &value){
    cout << value << " ";
}

void test(){
    vector<int>number = {1, 3, 5,7, 9, 8, 6, 4, 2, 10};
    //匿名函数--->lambda表达式
    //[]捕获列表
    //函数的返回值类型，可以省略
    for_each(number.begin(), number.end(), [](int &value){
        cout << ++value << " ";
    });
    cout<<endl;
    //将for_each前面的两个参数范围的元素作为print的参数
    for_each(number.begin(), number.end(), print);//这里是函数不是类所以不用加()
    cout<<endl;
    copy(number.begin(), number.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}

void display(int value){
    cout << value << " ";
}

void test2(){
    vector<int>number = {1, 3, 5, 7, 9, 8, 6, 4, 2, 10};
    for_each(number.begin(), number.end(), display);//这里是函数不是类所以不用加()
    cout<<endl;

    cout <<endl <<"count算法的使用"<<endl;
    size_t cnt = count(number.begin(), number.end(), 3);
    // number.count(3);
    cout << "cnt = " << cnt << endl;

    cout <<endl <<"find算法的使用"<<endl;
    auto it = find(number.begin(), number.end(), 6);
    if(it == number.end()){
        cout << "not find" <<endl;
    }else{
        cout << "find success: "<< *it <<endl;
    }
}

int main()
{
    test();
    test2();
    return 0;
}

