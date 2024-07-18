#include <iostream>
#include<list>
#include<unordered_map>
#include<utility>
#include<iterator>

using std::list;
using std::unordered_map;
using std::pair;
using std::endl;
using std::ostream_iterator;
using std::cout;

class LRUCache {
public:
    LRUCache(int capacity) 
    :_capacity(capacity)
    {

    }
    
    int get(int key) {//如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1 。
        auto it = _HashMap.find(key);
        if(it != _HashMap.end()){
            if(it->second.second != _keyList.end())
            {
                _keyList.erase(it->second.second);
                _keyList.push_back(key);
                _HashMap[key].second = --_keyList.end();
                return it->second.first;
            }
        }
        return -1;
    }
    
    void put(int key, int value) {//如果关键字 key 已经存在，则变更其数据值 value ；如果不存在，则向缓存中插入该组 key-value 。如果插入操作导致关键字数量超过 capacity ，则应该 逐出 最久未使用的关键字。
        if(get(key) != -1){
            _HashMap[key].first = value;
            return; 
        }else{
            if(_HashMap.size() < _capacity){
                insert(key, value);
            }else{
                int keyListFront = _keyList.front();
                _keyList.pop_front();
                _HashMap.erase(keyListFront);
                insert(key, value);
            }
        }
    }
    void print(){
        for(auto i: _HashMap){
            cout<<i.first << "  " << i.second.first <<endl;
        } 
    }

private:
    int _capacity;//容量
    list<int> _keyList;//按照最近使用的顺序存储键值对，靠近尾部是最近使用的，靠头近部的是最久未使用的
    unordered_map<int, pair<int, list<int>::iterator>> _HashMap;//存储关键字， 和存储用户输入值和链表的位置
    void insert(int key, int value){
        _keyList.push_back(key);
        _HashMap[key] = make_pair(value, --_keyList.end());
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */


int main()
{
    LRUCache * lRUCache = new LRUCache(2);

    lRUCache->put(2, 1); // 缓存是 {1=1, 2=2}
    lRUCache->put(2, 2); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    lRUCache->get(2);    // 返回 -1 (未找到)
    lRUCache->put(1, 1); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    lRUCache->put(4, 1);
    lRUCache->get(2);    //
    lRUCache->print();  
    return 0;
}

