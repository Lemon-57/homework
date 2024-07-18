 #include <iostream>
#include <string>

using namespace std;

//定义外部类
class Outer {
public:
    string name;
    static int age;
    void getName() {
        cout << "outer name is " << name << endl;
    }
    void getInnerName() {
        Inner inner;  //实例化内部类
        inner.name = "wan";
        inner.getName();
    }
    //定义内部类
    class Inner {
    public:
        string name;
        void getName() {//具有相同名字的变量，优先访问内部类的成员
            cout << "inner name is " << name << endl;  
        }
        void getOuterAge() {//内部类可直接访问外部类的静态成员
            cout << "outer name is " << age << endl;  
        }
        void getOuterName(Outer outer) {
            //此处通过传入的已经实例化的参数来访问外部类的非静态成员
            cout << "from inner outer name is " << outer.name << endl;  
        }
    };
};

int main() {
    //通过对象指针来获得内部类，外部不能直接访问内部类
    Outer::Inner *inner = new Outer::Inner();  
    inner->name = "inner";
    inner->getName();
    
    Outer outer;  //实例化外部类
    outer.name = "outer";
    outer.getName();
    outer.getInnerName();  //调用实例化内部类的函数，
}
