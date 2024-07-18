#include <iostream>
#include<cstring>
using std::cout;
using std::endl;


class Student{
public:
    Student(int length, int width)
    :_width(width)
    ,_length(length)
    {
    }
    void print(){
        cout<<(2*(_length+_width))<<(_length+_width)<<endl;
    }
    ~Student()
    {
        cout<<"~Student()"<<endl;
    }
private:
    int _length;
    int _width;
};


int main()
{
    Student st(2,3);
    st.print();
    return 0;
}

