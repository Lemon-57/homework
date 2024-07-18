#include <iostream>
#include<memory>
using namespace std;

struct FILECloser
{
    void operator()(FILE * fp) const
    {
        if(nullptr != fp){
            fclose(fp);
        }
    }
};

void test(){
    string msg = "hello, world\n";
    unique_ptr<FILE, FILECloser>up(fopen("wd.txt","a+"));
    fwrite(msg.c_str(), 1, msg.size(), up.get());
    //如果不加fclose，则没有写进文件，只是写在文件缓冲区
    // fclose(up.get());
}

void test1(){
    string msg = "hello, world\n";
    unique_ptr<FILE>up(fopen("wd.txt","a+"));
    fwrite(msg.c_str(), 1, msg.size(), up.get());
    //如果不加fclose，则没有写进文件，只是写在文件缓冲区
    fclose(up.get());
}

int main()
{
    test();
    return 0;
}

