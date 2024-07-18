#include <iostream>
#include<string>
#include<unordered_map>
#include<string>
#include<algorithm>
#include<fstream>
#include<vector>
using namespace std;


class Dictionary
{
private:
    unordered_map<string, int> word_count;
    vector<pair<string, int>>vecotr_word_count;
    static bool compare_vec(pair<string, int> &a, pair<string, int> &b)//为了消除this指针，写成全局函数或者static静态成员函数
    {
        return a.second < b.second;
    }

public:

    bool isWord(const string &word);
    void WordCount(string filename);

};

// bool compare_vec(pair<string, int> &a, pair<string, int> &b){
//     return a.second < b.second;
// }

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
    //while(ifs >> word)//iss -->bool/int
    while(ifs >> word, !ifs.eof())//逗号表达式
    {
        if(isWord(word)){
            auto ret = word_count.insert({word, 1});//插入单词，次数为1 ,返回值为键值对，且first为已存的键
            if(!ret.second){//second为bool值，当seconde为false时，则插入失败，表示已存在
                ++word_count[word];//对应的键值对的
            }    
        }
    }
    for(auto &w: word_count){
        vecotr_word_count.push_back(w);
    }
    sort(vecotr_word_count.begin(), vecotr_word_count.end(), compare_vec);
    for(auto &i: vecotr_word_count){
        cout << i.first << '\t' <<i.second << endl;
    }
    ifs.close();
}


int main()
{
    Dictionary dt;
    dt.WordCount("The_Holy_Bible.txt");
}

