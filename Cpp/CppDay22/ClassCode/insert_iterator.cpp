#include <iostream>
#include<iterator>
#include<vector>
#include<set>
#include<list>
using namespace std;

void test(){
    vector<int>vecNumber = {1, 5, 9, 11};
    list<int>listNumber = {22, 55, 99, 44};
    //将list的数据插入到vector尾部中
    copy(listNumber.begin(), listNumber.end(), back_inserter(vecNumber));
    copy(listNumber.begin(), listNumber.end(), back_inserter<vector<int>>(vecNumber));
    copy(vecNumber.begin(), vecNumber.end(), ostream_iterator<int>(cout, " "));
    cout<<endl;
    //将vector中的数据插入到list的头部
    copy(vecNumber.begin(), vecNumber.end(), front_inserter(listNumber));
    copy(vecNumber.begin(), vecNumber.end(), front_insert_iterator<list<int>>(listNumber));
    copy(listNumber.begin(), listNumber.end(), ostream_iterator<int>(cout, " "));
    cout<<endl;
    //将vector中数据插入到set中
    set<int>setNumber = {8, 9, 2, 3, 6, 33, 66};
    auto it = setNumber.begin();
    //copy(vecNumber.begin(), vecNumber.end(), inserter(setNumber, it));
    copy(vecNumber.begin(), vecNumber.end(), insert_iterator<set<int>>(setNumber, it));
    copy(setNumber.begin(), setNumber.end(), ostream_iterator<int>(cout, " "));
    cout<<endl;
}

void test1(){
    // vector<int>vec = {1, 2, 3, 4, 5, 6};
    // aut
}

int main()
{
    test();
    return 0;
}

