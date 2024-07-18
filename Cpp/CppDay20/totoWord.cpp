#include <iostream>
#include<string>
#include<map>
#include<string>
#include<algorithm>
#include<fstream>
using namespace std;
using std::cerr;
using std::ostream;

class Dictionary
{
private:
    map<string, int> word_count;
public:
    bool isWord(const string &word);
    void WordCount(string filename);
   

};

bool Dictionary::isWord(const string &word){
    bool flag = false;
    for(auto &i:word){
        if(isalpha(i)){
            flag = true;
        }
        else{
            flag = false;
        }
    }
    return flag;
}

void Dictionary::WordCount(string filename)
{
    ifstream ifs(filename);
    if(!ifs.good()){
        cerr <<"open file error\n";
        return;
    }

    string word;
    while(ifs >> word){
        if(isWord(word)){
            auto ret = word_count.insert({word, 1});//插入单词，次数为1
            if(!ret.second){//返回值为键值对，且first为已存的键，second为bool值，当seconde为false时，则插入失败，表示已存在
                ++word_count[word];//对应的键值对的
            }    
        }
    }

    for(auto &w: word_count){
        cout << w.first << '\t' <<w.second << endl;
    }
    ifs.close();
}


int main()
{
    Dictionary dt;
    dt.WordCount("The_Holy_Bible.txt");
}

