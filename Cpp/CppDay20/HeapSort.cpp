#include <iostream>
#include<vector>
#include<algorithm>
using namespace std;

template <typename T, typename Compare = std::less<T>>
class HeapSort
{
public:
    HeapSort(T *arr, size_t size, Compare com)
    :_vec(arr, arr + size)
    ,_len(size)
    {
        sort(com);
    }
    void headAdjust(size_t start, size_t end, Compare &com );
    void sort(Compare &com);
    void print(){
        for(auto &i:_vec){
            cout << i << " ";
        }
        cout << endl;
    }
private:
    vector<T> _vec;
    int _len;
};

//最大堆调整
template <typename T, typename Compare>
void HeapSort<T, Compare>::headAdjust(size_t start, size_t end, Compare &com ){
    int dad = start; //要调整的节点位置
    int son = 2 * dad + 1;
    while (son < end)//子节点在范围内才做比较 <= ?
    {//默认升序
        if(son + 1 < end && com(_vec[son], _vec[son+1]))//先比较两个子节点大小，选择最大的
        {
            son++;     
        }
        if(com(_vec[son], _vec[dad]))//父节点大于子节点中最大的则不调整
        {
            return;
        }
        else{//否则交换父子节点，交换后可能会破坏已排好的子节点和孙节点，需要再比较
            swap(_vec[dad], _vec[son]);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

template <typename T, typename Compare>
void HeapSort<T, Compare>::sort(Compare &com)
{
    //1.(创建最大堆)初始化，从最后一个小堆开始调整，也就是第一个父节点开始调整
    for(int start = _len/2 - 1; start >= 0; start--)
    {
        headAdjust(start, _len - 1, com);
    }
    //2.(最大调整堆)将堆首元素和堆尾元素互换，再调整堆
    //再将堆首元素和倒数第二个堆尾元素互换，再调整堆；
    //重复步骤一种调整到堆首，排序完毕
    for(int i = _len - 1; i > 0; i--)
    {
        swap(_vec[0],  _vec[i]);
        headAdjust(0, i, com);
    }
}

int main()
{
    int numbers[] = {1, 101, 89, 34, 45, 44, 90};
    HeapSort<int, std::less<int>> st1(numbers, sizeof(numbers)/sizeof(int), std::less<int>());
    st1.print();
    HeapSort<int, std::greater<int>> st2(numbers, sizeof(numbers)/sizeof(int), std::greater<int>());
    st2.print();
    return 0;
}

