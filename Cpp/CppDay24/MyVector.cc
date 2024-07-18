#include <iostream>
#include<algorithm>

using std::cout;
using std::endl;
using std::copy;

template<typename T>
class Vector
{
public:
    Vector()
    :_start(nullptr)
    ,_finish(nullptr)
    ,_end_of_storage(nullptr)
    {

    }
/*     using iterator = T *; */
    typedef T * iterator;
    iterator begin() 
    {
        return _start;
    }
    iterator end() 
    {
        return _end_of_storage;
    }


    ~Vector(); 
    void push_back(const T &value); 
    void pop_back();    

    int size() const;
    int capacity() const;

private:
    void reallocate();//重新分配内存,动态扩容要用的

private:    
    static std::allocator<T> _alloc;
    
    T *_start;                 //指向数组中的第一个元素
    T *_finish;                //指向最后一个实际元素之后的那个元素
    T *_end_of_storage;        //指向数组本身之后的位置
};

//静态数据成员只能在类外初始化
template<typename T>
std::allocator<T>Vector<T>::_alloc;

template<typename T>
Vector<T>:: ~Vector()
{
    if(_start){
        //将老的空间元素拷贝到新的空间
        while (_finish != _start)
        {
            //将老的空间上的元素销毁掉
            _alloc.destroy(--_finish);
        }
        //将老的空间回收掉
        _alloc.deallocate(_start, capacity());
    }
}

template<typename T>
void Vector<T>::push_back(const T &value){
    if(size() == capacity()){
        //空间的申请
        reallocate();//自己实现
    }
    if(size() < capacity()){
        _alloc.construct(_finish++, value);//构建对象
    }
}

template<typename T>
void Vector<T>::pop_back(){
    if(size() > 0){
        _alloc.destroy(--_finish);
    }
}

template<typename T>
int Vector<T>::size() const{
    return _finish - _start;
}
template<typename T>
int Vector<T>::capacity() const{
    return _end_of_storage - _start;
}

template<typename T>
void Vector<T>::reallocate()//重新分配内存,动态扩容要用的
{
    //扩容，老的空间，新的空间
    int oldCapacity = capacity();
    int newCapacity = oldCapacity > 0 ? 2 * oldCapacity : 1;
    //申请新的空间
    T * tmp = _alloc.allocate(newCapacity);
    if(_start){
        //将老的空间元素拷贝到新的空间
        copy(_start, _finish, tmp);
        while (_finish != _start)
        {
            //将老的空间上的元素销毁掉
            _alloc.destroy(--_finish);
        }
        //将老的空间回收掉
        _alloc.deallocate(_start, oldCapacity);
    }
    //将三个指针重新置位
    _start = tmp;
    _finish = tmp + oldCapacity;
    _end_of_storage = tmp + newCapacity;
}

template<typename Contrainer>
void display(const Contrainer &con) {
    cout << "con.szie()     " << con.size() << endl
         << "con.capacity()     " << con.capacity() << endl;
}

void test(){
    Vector<int>number;
    display(number);
    cout <<endl;

    number.push_back(1);
    display(number);
    cout <<endl;

    number.push_back(2);
    display(number);
    cout <<endl;

    number.push_back(3);
    display(number);
    cout <<endl;

    number.push_back(4);
    display(number);
    cout <<endl;

    number.push_back(5);
    display(number);
    cout <<endl;

    for(auto &elem : number){
        cout << elem << " ";
    }
    cout << endl;
}

int main()
{
    test();
    return 0;
}

