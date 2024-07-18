#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include<algorithm>

using namespace std;
class QueryResult;

class TextQuery
{
public://给一个由string组成的vector里的size_type起别名为line_no
        using line_no = vector<string>::size_type;
		TextQuery(ifstream&);//接收一个文件输入流对象并将它保存到wordmap中
        QueryResult query(const string&)const;//在worfmap中查询传入的单词
private:
        shared_ptr<vector<string>> file; //输入文件
        //单词为 键 ，指向保存了该单词出现过所有行号的set容器的指针为 值
        map<string, shared_ptr<set<line_no>>> wordmap;//每个单词到它所在行号的映射
        //一个word 对应一个 set，一个set里面存储里所有单词出现过不所有行号，由于set的特性不会有重复的行
};

class QueryResult {
    friend ostream &print(ostream &,const QueryResult &);
public://构造函数唯一的工作就是将参数保存到对应的数据成员中
    QueryResult(string s,
                shared_ptr<set<TextQuery::line_no>> p,
                shared_ptr<vector<string>> f)
                : sought(s)
                , lines(p)
                , file(f)
                {
                }
    set<TextQuery::line_no>::iterator begin() { return lines->begin(); }
	set<TextQuery::line_no>::iterator end() { return lines->end(); }
	shared_ptr<vector<string>> get_file() { return file; }
private:
    string sought;//保存查询单词
    shared_ptr<set<TextQuery::line_no>> lines;//指向保存某个单词出现的所有行号的set
    shared_ptr<vector<string>> file;//指向保存输入的文件
};
//词频统计
TextQuery::TextQuery(ifstream &is)
: file(new vector<string>)//初始化指向文件的智能指针，开辟一个新的vector来保存文本
{
    string text;
    while(getline(is, text)) {//每次从is中读取一行存到text中
        file->push_back(text);//将text存进file所指向的vector<string>中
        int n = file->size() - 1;//行号:从第0行开始，每次写入一行，file->size在不断增大，行号也在更新
        istringstream line(text);//从text中读取string到line流对象中 
        string word;
        while(line >> word)//解析line >> 输入流运算符遇到空格停止
        {                                   
            auto &lines = wordmap[word];//lines是一个智能指针 从wordmap中读取单词指向set的指针
            if(!lines)//对应值为0,则表示第一次遇到单词
                lines.reset(new set<line_no>);//分配一个新的set
            lines->insert(n);//将此行号插入set中
        }
    }
}

QueryResult TextQuery::query(const string& sought) const
{
    //如果未找到sought，则返回一个指向此set的指针
    static shared_ptr<set<line_no>> nodata(new set<line_no>);//static 延长nodata的生命周期
    //使用find来查找单词，避免添加单词到wordmap
    auto loc=wordmap.find(sought);
    if(loc == wordmap.end())
    {
        //未找到,返回(传入单词,空set，输入的文件)
        return QueryResult(sought, nodata, file); //返回局部变量所以要加static
    }else {//找到返回(传入单词，set,输入的文件)
        return QueryResult(sought, loc->second, file);
    }
}
//如果单词出现的此大于1次，返回 次数 time s，等于1 ，放回 time
string make_plural(size_t ctr, const string &word, const string &ending)
{
    return (ctr > 1) ? word + ending : word;
}
//将打印信息拷贝到os
ostream &print(ostream &os,const QueryResult &qr)
{//单词 occurs 行号 set的大小就是次数 
    os << qr.sought << " occurs " << qr.lines->size() << " "
       << make_plural(qr.lines->size(),"time","s") << endl;
    for (auto num : *qr.lines) //从set中读取出现的函数
        os << "\t(line " << num + 1 << ") "//存储的时候从0开始，打印的时候从1开始
           << *(qr.file->begin() + num) << endl;//按读取的行偏移文件指针
    return os;   
}
//不希望用户或者派生类直接使用Query_base，使用Query_base必须通过Query对象
class Query_base {
    friend class Query;
protected:
   //使用TextQuery中的line_no等价using line_no = vector<string>::size_type;
    using line_no = TextQuery::line_no;//用于eval函数
    virtual ~Query_base() = default; //隐式的在派生类内部
private:
    virtual QueryResult eval(const TextQuery&) const =0;//根据传入的TextQuery对象查询
    virtual string rep() const = 0;//使用查询的Query对象返回查询的表达式
};
//接口类 负责隐藏整个继承的类
class Query {
    friend Query operator~(const Query &);
    friend Query operator|(const Query&,const Query&);
    friend Query operator&(const Query&,const Query&);
public:
    //构建一个新的wordQuery
    Query(const string &s);
    //接口函数：调对应调Query_base函数
    QueryResult eval(const TextQuery &t) const
    {
        return q->eval(t);
    }
    std::string rep() const 
    {
        return q->rep();
    }
private:
    Query(shared_ptr<Query_base> query)
    : q(query)
    {

    }
    shared_ptr<Query_base> q;//每个Query对象都含有一个指向Query_Base的指针
};                                //因为Query是创建Query对象的唯一接口
//单词查询
class WordQuery
: public Query_base
{
    friend class Query;
    WordQuery(const string &s)
    : query_word(s)
    {

    }
    //定义所以继承来到虚函数
    QueryResult eval(const TextQuery &t) const
    {
        return t.query(query_word);
    }
    string rep() const 
    { 
        return query_word;
    }
private:
    string query_word;
};
//逻辑非查询
class NotQuery
: public Query_base
{
    friend Query operator~(const Query &);
    NotQuery(const Query &q)
    : query(q)
    {

    }
    string rep() const
    { 
        return "~(" + query.rep() + ")";
    }
    QueryResult eval(const TextQuery & text) const;
private:
    Query query;
};
//BinaryQuery 没有定义eval，而是继承了该纯虚函数，所以BinaryQuery也是一个抽象基类
class BinaryQuery 
: public Query_base
{
protected:
    BinaryQuery(const Query &l,const Query &r,string s)
    : lhs(l)
    , rhs(r)
    , opSym(s)
    {

    }
    string rep() const
    { 
        return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";
    }
    Query lhs, rhs;
    string opSym;
};
//继承了rep函数，但是覆盖了eval函数
//逻辑与查询
class AndQuery 
: public BinaryQuery
{
    friend Query operator & (const Query&,const Query &);
    AndQuery(const Query &left,const Query &right)
    : BinaryQuery(left, right, "&")
    {

    }
    QueryResult eval(const TextQuery&) const;
};
//逻辑或查询
class OrQuery 
: public BinaryQuery
{
    friend Query operator | (const Query&,const Query &);
    OrQuery(const Query &left,const Query &right)
    : BinaryQuery(left,right,"|")
    {

    }
    QueryResult eval(const TextQuery&) const;
};

inline Query::Query(const string &s)
: q(new WordQuery(s))//给初始化的WorQuery类并分配堆空间，
{                    //并Query_base基类的智能指针指向派生类

}

inline Query operator~(const Query &operand)
{
    return shared_ptr<Query_base>(new NotQuery(operand));
}

inline Query operator&(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new AndQuery(lhs,rhs));
}

inline Query operator|(const Query &lhs,const Query &rhs)
{
    return shared_ptr<Query_base>(new OrQuery(lhs,rhs));
}

QueryResult OrQuery::eval(const TextQuery& text) const
{
    //通过Query成员lhs和rhs进行虚调用
    //调用eval返回每个运算对象的QueryResult
    auto right = rhs.eval(text), left = lhs.eval(text);
    //ret_lines指向一个 存储单词所有行号的set  
    auto ret_lines = make_shared<set<TextQuery::line_no>>(left.begin(),left.end());
    //往set中再插入包含右操作数单词的所有行号
    ret_lines->insert(right.begin(),right.end());
    //在包含lhs和rhs的所有行号中查找，返回一个新的QueryResult，它表示lhs和rhs的并集
    return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text) const
{
    auto right = rhs.eval(text),left = lhs.eval(text);
    auto ret_lines = make_shared<set<TextQuery::line_no>>();//初始化
    //set_intersection求两个已排序范围的交集,并插入到*ret_lines中
    set_intersection(left.begin(), left.end(),
                     right.begin(), right.end(),
                     inserter(*ret_lines, ret_lines->begin()));
    //将两个范围的交集一起写入一个目的的迭代器中
    return QueryResult(rep(),ret_lines,left.get_file());
}

QueryResult NotQuery::eval(const TextQuery& text) const
{
    auto result = query.eval(text);//查找包含单词的所有行
    auto ret_lines = make_shared<set<TextQuery::line_no>>();//初始化
    auto beg = result.begin(), end = result.end();
    auto sz = result.get_file()->size();//文件的总行数
    for(size_t n = 0; n != sz; ++n)
    {                               //如果开始一行就是最后一行，beg++后面没有数据了
        if(beg == end || *beg != n)//或者 和result不相等的行
            ret_lines->insert(n);  //如果不在result当中，就添加这一行
        else if(beg != end)
            ++beg;  
    }
    return QueryResult(rep(), ret_lines, result.get_file());
}

std::ostream & operator << (ostream &os,const Query &query)
{
    return os << query.rep();
}

void ReadFile(ifstream &file)
{
    TextQuery tq(file);
    while (true)
    {
        begin:
        cout << "enter word to look for,or q to quit"<<endl;
        string s;
        if (!(cin >> s) || s == "q" )//如果输入的字符不为空.第一个条件为假，判断第二个条件 
        {
            break;
        }
        //解析命令
        if(s[0] == '~')
        {
            s.replace(0, 1, "");//将第一个 ~ 字符替换成空白
            Query q = ~Query(s);
            QueryResult locs = q.eval(tq);
            print(cout,locs) << endl;
            continue;
        }
        for(int i = 0; i < (int)s.size(); i++)
        {
            if(s[i] == '|')
            {
                string left(s.substr(0, i));
                string right(s.substr(i+1, s.size()-i-1));
                Query q = Query(left) | Query(right);
                QueryResult locs = q.eval(tq);
                print(cout,locs) << endl;
                goto begin;
            }
            else if(s[i] == '&')
            {
                string left(s.substr(0,i));
                string right(s.substr(i+1,s.size()-i-1));
                Query q = Query(left) & Query(right);
                QueryResult locs = q.eval(tq);
                print(cout,locs) << endl;
                goto begin;
            }
        }
        print(cout, tq.query(s)) << endl;
    }
}

int main(void)
{
    string filename;
    cout << "please enter filename:" <<endl;
    cin >> filename;
    ifstream file(filename);
    if(!file)
    {
        cout << "filename error" << endl;
        return -1;
    }
    ReadFile(file);//通过文件输入流
    return 0;
}


