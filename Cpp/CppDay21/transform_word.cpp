#include <iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<map>
#include<stdexcept>
#include<sstream>
using namespace std;

class WordMap
{
private:
    map<string, string>trans_map;//保存转换规则

public:
    //读取规则文件，建立转换映射
    void buildMap(ifstream &map_file){
        string key; //要转换的单词
        string value; //转换后的单词
        //读取第一个单词到key中，行中剩下规则内容存到value中
        //一行为一个规则 单词 空格 替换的单词
        while(map_file >> key && getline(map_file, value)){
            if(value.size() > 1){
                trans_map[key] = value.substr(1);//规则前面有个空格，从空格后提取到字符串结束
            }else{
                throw runtime_error("no rule for word");
            }
        }
    }
    const string transform(const string &word){
        auto word_pair = trans_map.find(word);
        if(word_pair != trans_map.cend()){
            return word_pair->second;//返回规则
        }else{
            return word;
        }
    }
    bool isWord(const string word){
        bool flag = false;
        for(auto i:word){

            if(isalpha(i)){
                flag = true;
            }else{
                return flag;
            }
        }
        return flag;
    }
    void word_transform(ifstream &map_file, ifstream &map_rule){
        buildMap(map_rule);
        string word;
        string line;
        while (getline(map_file, line))
        {
            istringstream text_line(line);
            while(text_line >> word){
                if(isWord(word)){
                    word = transform(word);
                    cout<< word;
                    cout<<" ";
                }
            }
            cout<<endl;
        }
    }
};

int main()
{
    WordMap wm;
    ifstream ifs_file("file.txt");
    ifstream ifs_rule("map.txt");
    wm.word_transform(ifs_file, ifs_rule);
    std::cout << "Hello world" << std::endl;
    return 0;
}

