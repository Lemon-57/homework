#include <iostream>
#include<log4cpp/Category.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/BasicLayout.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PatternLayout.hh>

#define MYLOGFATAL(msg) log->fatal(msg, __FILE__, __LINE__, __FUNCTION__);
#define MYLOGCRIT(msg) log->crit(msg, __FILE__, __LINE__, __FUNCTION__);
#define MYLOGERROR(msg) log->error(msg, __FILE__, __LINE__, __FUNCTION__);
#define MYLOGWARN(msg) log->warn(msg, __FILE__, __LINE__, __FUNCTION__);
#define MYLOGINFO(msg) log->info(msg, __FILE__, __LINE__, __FUNCTION__);
#define MYLOGDEBUG(msg) log->debug(msg, __FILE__, __LINE__, __FUNCTION__);

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
    void fatal(const char *msg, const char *file ,int line, const char * function);
    //void fatal(const char *msg);
    void crit(const char *msg, const char *file, int line, const char * function);
	void error(const char *msg, const char *file, int line, const char * function);
	void warn(const char *msg, const char *file, int line, const char * function);
	void info(const char *msg, const char *file, int line, const char * function);
	void debug(const char *msg, const char *file, int line, const char * function);
    
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

void Mylogger::fatal(const char *msg, const char *file, int line, const char * function){
    char logMsg[1024] = {0};
    sprintf(logMsg, "%s  %s  %d %s", msg, file, line, function);
    testModule.fatal(logMsg);
}

void Mylogger::crit(const char *msg, const char *file, int line, const char * function){
    char logMsg[1024] = {0};
    sprintf(logMsg, "%s  %s  %d %s", msg, file, line, function);
    testModule.crit(logMsg);
}

void Mylogger::error(const char *msg, const char *file, int line, const char * function){
    char logMsg[1024] = {0};
    sprintf(logMsg, "%s  %s  %d %s", msg, file, line, function);
    testModule.error(logMsg);
}

void Mylogger::warn(const char *msg, const char *file, int line, const char * function){
    char logMsg[1024] = {0};
    sprintf(logMsg, "%s  %s  %d %s", msg, file, line, function);
    testModule.warn(logMsg);
}

void Mylogger::info(const char *msg, const char *file, int line, const char * function){
    char logMsg[1024] = {0};
    sprintf(logMsg, "%s  %s  %d %s", msg, file, line, function);
    testModule.info(logMsg);
}

void Mylogger::debug(const char *msg, const char *file, int line, const char * function){
    char logMsg[1024] = {0};
    sprintf(logMsg, "%s  %s  %d %s", msg, file, line, function);
    testModule.debug(logMsg);
}

int main(void)
{
    Mylogger *log = Mylogger::getInstance();
    log->init();
    MYLOGFATAL("The log is fatal message");
    MYLOGCRIT("The log is crit message");
    MYLOGERROR("The log is error message");
    MYLOGWARN("The log is warn message");
    MYLOGINFO("The log is info message");
    MYLOGDEBUG("The log is debug message");
    Mylogger::destroy();
}

//输出的日志信息中最好能有文件的名字，函数的名字及其所在的行号(这个在C/C++里面有对应的宏，可以查一下)
