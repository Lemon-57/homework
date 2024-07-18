#include<iostream>
using namespace std;

int main(){
    int num = 0;
    cin>>num;
    int i = 0;
    while(num){
        num = num&(num -1);
        i++;
    }
    cout<<i<<endl;
    return 0;
}
