#include<iostream>
#include<stdio.h>
#include<log4cpp/Category.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/BasicLayout.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PatternLayout.hh>

#define LogInfo(...) log->info(__VA_ARGS__)

using namespace std;
using namespace log4cpp;


//Mylogger.cc 使用单例模式
class Mylogger
{
public:
    Mylogger(const Mylogger &) = delete;
    Mylogger & operator = (const Mylogger &) = delete;
    //3.定义一个返回值为类指针的静态成员函数
    static Mylogger * getInstance()
    {
        if(nullptr == _pInstance){//判断是否是第一次使用
            _pInstance = new Mylogger();    
        }
        return _pInstance;
    }
    static void destroy(){
        if(_pInstance){
            delete _pInstance;
        }
    }
    void init();
    template<typename ...Args>
	void info(Args...args);
    
private:
    //1.将构造、析构函数私有化
    Mylogger(){
        cout << "Mylogger" << endl;
    }
    ~Mylogger(){
    //    Category::shutdown();
        cout << "~Mylogger" <<endl;
    }
    
private:
    //2.在类中定义一个静态的指向本类型的指针变量，
    //程序结束时会自动调用它的析构函数,删除单例类的实例
    static Mylogger *_pInstance;
};

Category & testModule = Category::getRoot().getInstance("TestModule");
Mylogger * Mylogger::_pInstance = nullptr;

void Mylogger::init(){
    //1.设置布局
    auto ptnLayout = new PatternLayout();
    ptnLayout->setConversionPattern("%d [%c] [%p] %m%n");
    //2.设置日志目的地,采用追加的模式写入
    auto pFileAppender = new FileAppender("fileApp", "lemon.log");
    pFileAppender->setLayout(ptnLayout);
    //3.创建日志的来源对象
    //设置优先级
    testModule.setPriority(Priority::DEBUG);
    testModule.addAppender(pFileAppender);
}

template<typename ...Args>
void Mylogger::info(Args ... args){
    char logMsg[1024] = {0};
    sprintf(logMsg, args...);
    testModule.info(logMsg);
}


void test() 
{
	Mylogger *log = Mylogger::getInstance();
    log->init();
	int number = 100;
	const char *pstr = "hello, log4cpp";
	LogInfo("This is an info message. number = %d, str = %s\n", number, pstr);
    Mylogger::destroy();
}

int main(void)
{
    test();    
}

//输出的日志信息中最好能有文件的名字，函数的名字及其所在的行号(这个在C/C++里面有对应的宏，可以查一下)

