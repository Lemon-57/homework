#include <iostream>
#include <algorithm>
#include<vector>
#include<iterator>
using namespace std;

void print(int &value){
    cout << value << " ";
}

bool func(int value){
    return value > 5;
}

void test(){
    vector<int>number = {1, 3, 5, 7, 9, 8, 6, 4, 2, 10};
    for_each(number.begin(), number.end(),print);
    cout<<endl;
    // auto it = remove_if(number.begin(), number.end(),func);
    // auto it = remove_if(number.begin(), number.end(), [](int &value){
    //     return value > 5;
    // });
    //将一个二元断言转化一元断言
    auto it = remove_if(number.begin(), number.end(), std::bind1st(std::less<int>(), 5));
    auto it = remove_if(number.begin(), number.end(), std::bind2nd(std::greater<int>(), 5));    
    number.erase(it, number.end());
    for_each(number.begin(), number.end(),print);
    cout<<endl;    
}

void test2(){
    vector<int> number;
    number.push_back(1);
    bool flag = true;
    for(auto it = number.begin(); it != number.end(); ++it){
        cout << *it << " ";//执行读操作
        if(flag){
            //底层发生扩容的时候，迭代器it已经失效了
            number.push_back(2);//执行写操作
            flag = false;
            //解决方案:将迭代器重新置位
            it = number.begin();
        }
    }
    cout << endl;
}

int main()
{
    test();
    //test2();
    return 0;
}

