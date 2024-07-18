#include <iostream>
#include<vector>

using std::vector;
using std::cin;
using std::endl;
using std::cout;

class sortArr{
public:
    sortArr(vector<int> arr)
    :_arr(arr)
    {
        arr.reserve(5);
        for(size_t idx = 0; idx < arr.capacity(); ++idx){
            cin >> arr[idx];
            _arr.emplace_back(arr[idx]);
        }
    }
    void sort();
    bool is_odd(int n);
private:
    vector<int>_arr;
};

void sortArr::sort(){
    for(size_t idx = 0; idx <  _arr.capacity(); ++idx){
        if(!is_odd(_arr[idx]) && is_odd(_arr[idx+1])){
            _arr[idx] = _arr[idx] ^ _arr[idx+1];
            _arr[idx+1] = _arr[idx] ^ _arr[idx+1];
            _arr[idx] = _arr[idx] ^ _arr[idx+1];
        }else{
            continue;
        }    
    }
    
    for(auto i:_arr){
        cout<< i << endl;
    }
}

bool sortArr::is_odd(int n){
    return n & 0x1; //1奇数 0偶数
}

int main()
{
    vector <int> arr;
    sortArr Arr(arr);
    Arr.sort();
    return 0;
}

