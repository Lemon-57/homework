#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include<fstream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;
using std::ostream;
using std::regex;
struct RssItem
{
    string docid;
    string title;
    string link;
    string description;
    string content;
};


class RssReader
{
public:
    RssReader()
    {
        
    };
    void parseRss();//解析
    void dump(const string & filename);//输出
private:
    vector<RssItem> _rss;
};

string delete_spilth(string line)
{
    regex rx(R"(\<.+?\>|&nbsp)");   
    return regex_replace(line, rx,""); //替换匹配，将符合rx规则的字符串换成""
}

void RssReader::parseRss(){
    XMLDocument doc;
    if(doc.LoadFile("./cooshell.xml") != XML_SUCCESS)//加载文件
    {
        cout << "LoadFile Fail!" << endl;
        return;
    }
    cout << "LoadFile Success!"<<endl;

    XMLElement * root = doc.RootElement();//xml的根节点
    if(NULL == root){
        cout <<"find root fail"<<endl;
    }
    XMLElement * root_channel = root->FirstChildElement("channel");//第一个channel节点
    XMLElement * root_item = root_channel->FirstChildElement("item");//chnanel节点的第一个item

    while(root_item){
        RssItem rss;
        XMLElement * root_title = root_item->FirstChildElement("title");
        rss.title  = root_title->GetText();
        //string title = 
        XMLElement * root_link = root_item->FirstChildElement("link");
        rss.link = root_link->GetText();
        
        XMLElement * root_description = root_item->FirstChildElement("description");
        rss.description = delete_spilth(root_description->GetText());
    
        XMLElement * root_content = root_item->FirstChildElement("content:encoded");
        rss.content = delete_spilth(root_content->GetText());

        _rss.push_back(rss);
        root_item = root_item->NextSiblingElement("item");//找到下一个item
    }
    cout <<"parseRss over! "<<endl;
}

void RssReader::dump(const string & filename)//输出到文件中
{
    ofstream ofs;
    ofs.open(filename);
    if(!ofs.good()){
        cerr << "ofstream open file error\n";
        return;
    }
    int size = _rss.size();
    for(int i = 0; i < size; i++){
        ofs << "<doc>" << endl; 
        ofs << "     " << "<docid>" << i+1 << "<docid>" <<endl;
        ofs << "     " << "<title>" << _rss[i].title << "<title>" << endl;
        ofs << "     " << "<link>" <<_rss[i].link << "<link>" << endl;
        ofs << "     " << "<description>" << _rss[i].description << "<description>" << endl;
        ofs << "     " << "<content>" << _rss[i].content << "<content>" <<endl;
    }
    ofs.close();
}


int main()
{
    RssReader rss;
    rss.parseRss();
    rss.dump("rss.txt");
    return 0;
}

