#include <iostream>
#include<list>
#include<algorithm>
#include<string>
#include<memory>

using std::cout;
using std::endl;
using std::list;
using std::find;
using std::string;
using std::unique_ptr;

class Observer;

class Subject{
public:
    virtual void attach(Observer *) = 0;
    virtual void detach(Observer *) = 0;
    virtual void notify() = 0;
    virtual ~Subject(){

    } 
};

class ConcreteSubject
:public Subject
{
public:
    void attach(Observer * pObserver) override{//添加被观察者
        _oblist.push_back(pObserver);
    }
    void detach(Observer * pObserver) override{
        for(auto it = _oblist.begin(); it != _oblist.end(); ++it){
            if(*it == pObserver){
                _oblist.remove(pObserver);//删除指定的被观察者
                break;
            }
        }
    }
    void notify() override;

    void setStatus(int status){
        _status = status;
    }
    int getStatus() const{
        return _status;
    }
private:
    list<Observer * > _oblist;
    int _status;
};

class Observer{
public:
    virtual void update(int status) = 0;
    virtual ~Observer(){

    }
};

class ConcreObserver1
:public Observer
{
public:
    ConcreObserver1(const string &name)
    :_name(name)
    {
    
    }
    void update(int status) override{
        cout << "ConcreObserver1 = " << _name << ", value = " << status <<endl;
    }
private:
    string _name;
};

class ConcreObserver2
:public Observer
{
public:
    ConcreObserver2(const string &name)
    :_name(name)
    {
    
    }
    void update(int status) override{
        cout << "ConcreObserver2 = " << _name << ", value = " << status <<endl;
    }
private:
    string _name;
};

void ConcreteSubject::notify() {//通知所有的被观察者
    for(auto &ob : _oblist){
        ob->update(_status);//更新被观察者的状态
    }
}

int main()
{
    unique_ptr<ConcreteSubject>pSubject(new ConcreteSubject());
    unique_ptr<Observer>pObserver1(new ConcreObserver1("lq"));
    unique_ptr<Observer>pObserver2(new ConcreObserver2("ww"));

    pSubject->setStatus(2);
    pSubject->attach(pObserver1.get());
    pSubject->attach(pObserver2.get());

    pSubject->notify();

    pSubject->detach(pObserver2.get());
    puts("----------------------");
    pSubject->setStatus(3);
    pSubject->notify();

    return 0;
}

