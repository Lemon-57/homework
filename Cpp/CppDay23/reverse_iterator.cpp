#include <iostream>
#include<vector>
#include<iterator>

using namespace std;

void test(){
    vector<int>number = {1, 3, 5,7, 9,8,6,4,2,10};
    vector<int>::iterator it = number.begin();
    for(; it != number.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;
}

void test1(){
    vector<int>number = {1, 3, 5,7, 9,8,6,4,2,10};
    vector<int>::reverse_iterator rit = number.rbegin();//反向迭代器
    for(; rit != number.rend(); ++rit){
        cout << *rit << " ";
    }
    cout << endl;
}

int main()
{
    test();
    test1();
    return 0;
}

