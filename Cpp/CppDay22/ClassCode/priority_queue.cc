#include <iostream>
#include<queue>
#include<vector>
#include<deque>
#include<math.h>
using namespace std;

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        /* cout << "Point(int = 0, int = 0)" << endl; */
    }

    double getDistance() const
    {
        return hypot(_ix, _iy);
    }

    int getX() const
    {
        return _ix;
    }

    int getY() const
    {
        return _iy;
    }

    ~Point()
    {
        /* cout << "~Point()" << endl; */
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &rhs);
    friend bool operator<(const Point &lhs, const Point &rhs);
    friend bool operator>(const Point &lhs, const Point &rhs);
    friend struct Comparation;
private:
    int _ix;
    int _iy;
};

std::ostream &operator<<(std::ostream &os, const Point &rhs)
{
    os << "(" <<  rhs._ix
       << ", " << rhs._iy
       << ")";

    return os;
}

bool operator<(const Point &lhs, const Point &rhs)
{
    //cout << "bool operator<(const Point &, const Point &)" << endl;
    if(lhs.getDistance() < rhs.getDistance())
    {
        return true;
    }
    else if(lhs.getDistance() == rhs.getDistance())
    {
        if(lhs._ix < rhs._ix)
        {
            return true;
        }
        else if(lhs._ix == rhs._ix)
        {
            if(lhs._iy < rhs._iy)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else 
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool operator>(const Point &lhs, const Point &rhs)
{
    //cout << "bool operator>(const Point &, const Point &)" << endl;
    if(lhs.getDistance() > rhs.getDistance())
    {
        return true;
    }
    else if(lhs.getDistance() == rhs.getDistance())
    {
        if(lhs._ix > rhs._ix)
        {
            return true;
        }
        else if(lhs._ix == rhs._ix)
        {
            if(lhs._iy > rhs._iy)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else 
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//函数对象（仿函数）
struct Comparation
{
    bool operator()(const Point &lhs, const Point &rhs) const
    {
        cout << "bool operator()(const Point &, const Point &) const" << endl;
        if(lhs.getDistance() < rhs.getDistance())
        {
            return true;
        }
        else if(lhs.getDistance() == rhs.getDistance())
        {
            if(lhs._ix < rhs._ix)
            {
                return true;
            }
            else if(lhs._ix == rhs._ix)
            {
                if(lhs._iy < rhs._iy)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else 
            {
                return false;
            }
        }
        else
        {
            return false;
        }

    }
};

//priority_queue优先级队列
void test(){
    vector<int>vec = {1, 4, 6, 8, 5, 4, 3, 7};
    // priority_queue<int>pque(vec.begin(), vec.end());
    // priority_queue<int>pque;
    // priority_queue<int, vector<int>, std::greater<int>>pque;
    priority_queue<int, deque<int>, std::greater<int>>pque;
    for(size_t idx = 0; idx != vec.size(); ++idx){
        pque.push(vec[idx]);
        cout<< "优先级最高的元素"<<pque.top() << endl;
    }
    //背后需要有迭代器的支持
    // for(auto &elem : pque){

    // }
    while (!pque.empty())
    {
        cout<< pque.top()<<" ";
        pque.pop();
    }
    cout << endl;
}

void test2()
{
    vector<Point>vec = {
        Point(1, 2),
        Point(1, -2),
        Point(-1, 2),
        Point(3, 2),
        Point(4, 5),
        Point(-2, 2),
        Point(3, 2),
        Point(1, 2),
    };

    priority_queue<int, vector<Point>, std::greater<Point>>pque;
    for(size_t idx = 0; idx != vec.size(); ++idx){
        pque.push(vec[idx]);
        cout<< "优先级最高的元素:"<<pque.top() << endl;
    }

    // priority_queue<int, vector<Point>, std::less<Point>>pque;
    // for(size_t idx = 0; idx != vec.size(); ++idx){
    //     pque.push(vec[idx]);
    //     cout<< "优先级最高的元素"<<pque.top() << endl;
    // }
    while (!pque.empty())
    {
        cout<< pque.top()<<" ";
        pque.pop();//
    }
    cout << endl;

}

int main()
{
    test();
    //test2();
    return 0;
}

