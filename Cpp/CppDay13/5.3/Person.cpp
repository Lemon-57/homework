#include <iostream>
#include<string>
using namespace std;

class Person{
public:
    Person(const string name, const int age)
    :_name(name)
    ,_age(age)
    {
        
    }
    void display(){
        cout <<"name: " << _name << endl;
        cout << "age" << _age << endl;
    }

private:
    string _name;
    int _age;

};

class Employee
:public Person
{
public:
    Employee(const string department, int salary, const string name, int age)
    :Person(name, age)
    ,_department(department)
    ,_salary(salary)
    {
        
    }
    void display(){
        Person::display();
        cout << "department: " << _department << endl;
        cout << "salary" << _salary << endl;
    }
    int salary(){
        return _salary;
    }
private:
    string _department;
    int _salary;
    string _name;
    int _age;
};



int main()
{
    Employee person1("IT", 8888, "qq", 18);
    Employee person2("Teacher", 5555, "ll", 18);
    Employee person3("Boss", 28888, "lq", 20);
    person1.display();
    person2.display();
    person3.display();
    cout << (person1.salary()+ person2.salary() + person3. salary())/3 <<endl;
    return 0;
}

