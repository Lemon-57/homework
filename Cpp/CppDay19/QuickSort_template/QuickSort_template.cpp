#include <iostream>
#include<vector>
using namespace std;

//template <classT, typename Compare = std::less<T>>就是说
//可以传入两个参数，一个T，一个Compare，但是第二个参数可以不传入，使用std::less<T>
template<typename T,typename Compare=std::less<T>>
class MyQsort
{
public:
    MyQsort(T *arr, size_t size, Compare com)
    :_vec(arr, arr+size)
    {
        quick(0, size - 1, com);
    }
	void quick(int left, int right, Compare &com);
	int partition(int left, int right, Compare &com);
    void print(){
        for(auto &i:_vec){
            cout << i << " "; 
        }
        cout << endl;
    }
private:
	vector<T> _vec;
};

template<typename T, typename Compare>
void MyQsort<T, Compare>::quick(int left, int right, Compare &com)
{
    int pivot;
    if(left < right){
        pivot = partition(left, right, com);
        quick(left, pivot - 1, com);
        quick(pivot + 1, right, com);
    }
    
}

template<typename T, typename Compare>
int MyQsort<T, Compare>::partition(int left, int right, Compare &com)//返回调整后基准数的位置
{
    int i = left;
    int j = right;
    int x = _vec[left];
    while(i < j)
    {
        //从右向找左小于x来填_vec[i],
        //while(i < j && _vec[j] >= x){
        while(i < j && com(x, _vec[j])){
            j--;
        }//出循环时一定是_vec[j] < x 交换位置
        if(i < j){
            _vec[i] = _vec[j]; 
            i++;
        }//从左向找右大于或等于x来填_vec[j]
        //while(i < j && _vec[i] < x){
        while(i < j && com(_vec[i], x)){
            i++;
        }//出循环时一定是_vec[i] >= x 交换位置
        if(i < j){
            _vec[j] = _vec[i];
            j--;
        }
    }
    _vec[i] = x;
    return i;
}


int main()
{
    int numbers[] = {1, 101, 89, 34, 45, 44, 90};
    MyQsort<int, std::less<int>> st1(numbers, 7, std::less<int>());
    st1.print();
    MyQsort<int, std::greater<int>> st2(numbers, 7, std::greater<int>());
    st2.print();
    return 0;
}

