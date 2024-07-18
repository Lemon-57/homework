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
public:
        using line_no = vector<string>::size_type;
		TextQuery(ifstream&);
        QueryResult query(const string&)const;
private:
        shared_ptr<vector<string>> file; //输入文件
        map<string,shared_ptr<set<line_no>>> wordmap;
};

class QueryResult {
    friend ostream &print(ostream &,const QueryResult &);
public:
    QueryResult(string s,
                shared_ptr<set<TextQuery::line_no>> p,
                shared_ptr<vector<string>> f)
                : sought(s)
                , lines(p)
                , file(f)
                {}
    set<TextQuery::line_no>::iterator begin() { return lines->begin(); }
	set<TextQuery::line_no>::iterator end() { return lines->end(); }
	shared_ptr<vector<string>> get_file() { return file; }
private:
    string sought;
    shared_ptr<set<TextQuery::line_no>> lines;
    shared_ptr<vector<string>> file;
};

TextQuery::TextQuery(ifstream &is)
: file(new vector<string>)
{
    string text;
    while(getline(is,text)){
        file->push_back(text);
        int n=file->size() - 1;
        istringstream line(text);
        string word;
        while(line >> word)
        {
            auto &lines = wordmap[word];
            if(!lines)
                lines.reset(new set<line_no>);
            lines->insert(n);
        }
    }
}

QueryResult TextQuery::query(const string& sought) const
{
    //如果未找到sought，则返回一个指向此set的指针
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
    //使用find来查找单词，避免添加单词到wordmap
    auto loc=wordmap.find(sought);
    if(loc == wordmap.end())
    {
        //未找到
        return QueryResult(sought,nodata,file); 
    }else {
        return QueryResult(sought,loc->second,file);
    }
}

string make_plural(size_t ctr,const string &word,const string &ending)
{
    return (ctr > 1)?word + ending : word;
}

ostream &print(ostream &os,const QueryResult &qr)
{
    os << qr.sought << " occurs " << qr.lines->size() << " "
       << make_plural(qr.lines->size(),"time","s") << endl;
    for (auto num : *qr.lines)
        os << "\t(line " << num + 1 << ") "
           << *(qr.file->begin() + num) << endl;
    return os;   
}

class Query_base {
    friend class Query;
protected:
    using line_no = TextQuery::line_no;
    virtual ~Query_base() = default;
private:
    virtual QueryResult eval(const TextQuery&) const =0;
    virtual string rep() const =0;
};
//接口类 负责隐藏整个继承的类
class Query {
    friend Query operator~(const Query &);
    friend Query operator|(const Query&,const Query&);
    friend Query operator&(const Query&,const Query&);
public:
    //构建一个新的wordQuery
    Query(const string&);
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
    {}

    std::shared_ptr<Query_base> q;
};

class WordQuery
: public Query_base
{
    friend class Query;
    WordQuery(const string &s)
    : query_word(s)
    {}
    //定义所以继承来到虚函数
    QueryResult eval(const TextQuery &t) const
    {
        return t.query(query_word);
    }

    string rep() const 
    { return query_word;}

private:
    string query_word;
};

class NotQuery
: public Query_base
{
    friend Query operator~(const Query &);
    NotQuery(const Query &q)
    : query(q)
    {}

    string rep() const
    { return "~(" + query.rep() + ")";}

    QueryResult eval(const TextQuery &) const;

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
    {}

    string rep() const
    { return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";}

    Query lhs,rhs;
    string opSym;
};
//继承了rep函数，但是覆盖了eval函数
class AndQuery 
: public BinaryQuery
{
    friend Query operator&(const Query&,const Query &);
    AndQuery(const Query &left,const Query &right)
    : BinaryQuery(left,right,"&")
    {}

    QueryResult eval(const TextQuery&) const;
};

class OrQuery 
: public BinaryQuery
{
    friend Query operator|(const Query&,const Query &);
    OrQuery(const Query &left,const Query &right)
    : BinaryQuery(left,right,"|")
    {}

    QueryResult eval(const TextQuery&) const;
};

inline Query::Query(const string &s)
: q(new WordQuery(s))
{}

inline Query operator~(const Query &operand)
{
    return shared_ptr<Query_base>(new NotQuery(operand));
}

inline Query operator&(const Query &lhs,const Query &rhs)
{
    return shared_ptr<Query_base>(new AndQuery(lhs,rhs));
}

inline Query operator|(const Query &lhs,const Query &rhs)
{
    return shared_ptr<Query_base>(new OrQuery(lhs,rhs));
}

QueryResult OrQuery::eval(const TextQuery& text) const
{
    auto right = rhs.eval(text),left = lhs.eval(text);
    auto ret_lines = make_shared<set<TextQuery::line_no>>(left.begin(),left.end());
    ret_lines->insert(right.begin(),right.end());
    //返回一个新的QueryResult，它表示lhs和rhs的并集
    return QueryResult(rep(),ret_lines,left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text) const
{
    auto right = rhs.eval(text),left = lhs.eval(text);
    auto ret_lines = make_shared<set<TextQuery::line_no>>();
    set_intersection(left.begin(),left.end(),
                     right.begin(),right.end(),
                     inserter(*ret_lines,ret_lines->begin()));
    //将两个范围的交集一起写入一个目的的迭代器中
    return QueryResult(rep(),ret_lines,left.get_file());
}

QueryResult NotQuery::eval(const TextQuery& text) const
{
    auto result = query.eval(text);
    auto ret_lines = make_shared<set<TextQuery::line_no>>();
    auto beg = result.begin(), end = result.end();
    auto sz = result.get_file()->size();
    for(size_t n=0;n!=sz;++n)
    {
        if(beg == end || *beg != n)
            ret_lines->insert(n);  //如果不在result当中，就添加这一行
        else if(beg != end)
            ++beg;
    }
    return QueryResult(rep(),ret_lines,result.get_file());
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
        if (!(cin>>s)||s=="q")
        {
            break;
        }
        //解析命令
        if(s[0] == '~')
        {
            s.replace(0,1,"");
            Query q = ~Query(s);
            QueryResult locs = q.eval(tq);
            print(cout,locs) << endl;
            continue;
        }
        for(int i=0;i<s.size();i++)
        {
            if(s[i] == '|')
            {
                string left(s.substr(0,i));
                string right(s.substr(i+1,s.size()-i-1));
                Query q = Query(left) | Query(right);
                QueryResult locs = q.eval(tq);
                print(cout,locs) << endl;
                goto begin;
            }else if(s[i] == '&')
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
    ReadFile(file);
    return 0;
}


