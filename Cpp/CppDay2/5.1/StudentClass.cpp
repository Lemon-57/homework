#include <iostream>
#include<cstring>
using std::cout;
using std::endl;


class Student{
public:
    Student(int id, const char *name, int age)
    :_id(id)
    ,_name(new char[strlen(name)+1]())
    ,_age(age)
    {
        strcpy(_name, name);
    }
    void print(){
        cout<<_id<<_name<<_age<<endl;
    }
    ~Student()
    {
        cout<<"~Student()"<<endl;
        delete[]_name;
        _name = nullptr;
    }
private:
    int _id;
    char *_name;
    int _age;
};


int main()
{
    Student st(1, "lq", 25);
    st.print();
    return 0;
}

