#include <iostream>
#include<cstring>
using std::cout;
using std::endl;

int _top = -1;
#define SIZE 10
class Stack{
public:
    Stack(int num[SIZE])
    {
        for(int i = 0; i < 10; i++){
            num[i] = -1;
        }
    }
    bool empty();
    bool full();
    void push(int);
    void pop();
    int top();
    void print(){
    }
    ~Stack()
    {
        cout<<"~Stack()"<<endl;
    }
private:
    int _num[SIZE];
};

bool Stack::empty(){
    if(_top  = -1){
        return 1;
    }
    else{
        return -1;
    }
}
bool Stack::full(){
    if(_top = 9){
        return 1;
    }
    else{
        return -1;
    }
}
void Stack::push(int snum){
    if(_top < 9){
        _top++;
        _num[_top] = snum;     
    }
    else{
        exit(0);
    }
}
void Stack::pop(){
    _top--;
}
int Stack::top(){
    return _num[_top];
}

int main()
{
    int num[10] = {0};
    Stack sk(num);
    sk.push(1);
    sk.push(2);
    sk.push(3);
    sk.push(3);
    sk.push(3);
    sk.push(3);
    sk.push(3);
    sk.push(3);
    sk.push(3);
    sk.push(3);
    sk.pop();
    cout <<sk.top()<<endl;
    sk.full();
    return 0;
}

