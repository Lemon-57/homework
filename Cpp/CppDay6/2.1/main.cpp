#include <iostream>
#include<fstream>
#include<limits>
using std::cin;
using std::cout;
using std::endl;
void readIneger(double &str){
    cout<<"please input integer or decimals "<<endl;
    while(cin >>str, !cin.eof()){
        if(cin.bad()){
            cout<<"bad"<<endl;
            return;
        }else if(cin.fail()){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            cout <<"fail"<<endl;
            continue;
        }else{
            cout<<"number:  "<<str<<endl;
            break;
        }
    }
}

int main()
{
    double str;
    readIneger(str);
    return 0;
}

