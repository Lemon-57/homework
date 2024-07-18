#include<iostream>
#include<vector>
using std::cout;
using std::endl;
using std::vector;

int main(){
    vector<int> arr{1,2,3,2,2,2,5,4,2};
    int size = 0;
    int x = 1;
    for(auto i : arr){
        if(x == i){
            size++;
        }else{
            size--;
            if(size < 0){
                x = i;
            }
        }
    }
    cout<<x<<endl;
    return 0;
}
