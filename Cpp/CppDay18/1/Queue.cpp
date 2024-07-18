#include <iostream>
using namespace std;

template <typename T, size_t Qsize = 10>
class Queue{
public:
    Queue()
    :_head(-1)
    ,_rear(-1)
    ,_data(new T[Qsize]())
    {
        
    }
    ~Queue(){
        if(_data){
            delete []_data;
            _data = nullptr;
        }
    }
    bool empty() const;
    bool full() const;
    void push(const T &t);
    void pop();
    T head() const;
    T rear() const;

private:
    int _head;
    int _rear;
    T * _data;
};

template<typename T, size_t Qsize>
bool Queue<T, Qsize>::empty() const{
    return _rear == _head;
}

template<typename T, size_t Qsize>
bool Queue<T, Qsize>::full() const{
    return _rear == Qsize - 1;
}

template<typename T, size_t Qsize>
void Queue<T, Qsize>::push(const T &t){
    if(!full()){
        if(_rear == Qsize - 1){
            for(int i = _head; i < _rear; i++ ){
                _data[i - _head] = _data[_rear - i];
            }
            _rear = _head;
        }
        _rear++;
        _data[_rear] = t;
    }else{
        puts("Queue is full");
    }
}

template<typename T, size_t Qsize>
void Queue<T, Qsize>::pop(){
    if(!empty()){
        _data[_head + 1] = -1;
        _head++; 
    }else{
        puts("Queue is empty");    
    }
}

template<typename T, size_t Qsize>
T Queue<T, Qsize>::head() const{
    return _data[_head];
}

template<typename T, size_t Qsize>
T Queue<T, Qsize>::rear() const{
    return _data[_rear];
}

int main()
{
    Queue<int, 10> st;
    if(st.empty()){
        cout<<"isEmpty"<<endl;
    }
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    st.push(5);
    st.push(6);
    st.push(7);
    st.push(8);
    st.push(9);
    st.push(10);
    st.pop();
    st.push(10);
    cout<<st.head()<<endl;
    cout<<st.rear()<<endl;
    if(st.full()){
        cout<<"isfull"<<endl;
    }
    return 0;
}

