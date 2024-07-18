#include <iostream>
#include<cstring>
using std::cout;
using std::endl;

#define SIZE 10
class Stack{
public:
    Stack(int arr[SIZE])
    {
        for(int i = 0; i < 10; i++){
            arr[i] = -1;
        }
    }
    bool empty();
    bool full();
    void push(int);
    void pop();
    int front();
    int back();
    ~Stack()
    {
        cout<<"~Stack()"<<endl;
    }
private:
    int arr[SIZE] = {0};
    int head = -1;
    int rear = -1;
    int size = 0;
};

bool Stack::empty(){
    if(size  = 0){
        return 1;
    }
    else{
        return -1;
    }
}
bool Stack::full(){
    if(size = 10){
        return 1;
    }
    else{
        return -1;
    }
}
void Stack::push(int num){
    if(size < 10){
        if(rear == 9){
            for(int i = head; i < rear; i++){
                arr[i-head] = arr[rear];
            } 
            rear = head;
        }
        size++;
        rear++;
        arr[rear] = num;     
    }
    else{
        exit(0);
    }
}
void Stack::pop(){
    size--;
    head++;
    arr[head] = -1;
}
int Stack::front(){
    return arr[head];
}
int Stack::back(){
    return arr[rear];
}

int main()
{
    int num[10] = {0};
    Stack sk(num);
    if(sk.empty()){
        cout<<"isEmpty"<<endl;
    }
    sk.push(1);
    sk.push(2);
    sk.push(3);
    sk.push(4);
    sk.push(5);
    sk.push(6);
    sk.push(7);
    sk.push(8);
    sk.push(9);
    sk.push(10);
    sk.pop();
    sk.push(10);
    cout<<sk.front()<<endl;
    cout<<sk.back()<<endl;
    if(sk.full()){
        cout<<"isfull"<<endl;
    }

    return 0;
}

