#include <iostream>
#include<vector>
#include<fstream>
#include<string>
#include<algorithm>
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::ifstream;
using std::ofstream;

struct Record
{
	string _word;
	int _frequency;
    Record(string word, int frequency)
    :_word(word)
    ,_frequency(frequency)
    {

    }
};

class Dictionary
{
public:
    bool isword(const string &word);
    void read(const string &filename);
    void store(const string &filename);
    void addDict(const string &word);
private:
	vector<Record> _dict;
};

bool comp(Record &word1, Record &word2);    

bool Dictionary::isword(const string &word){
    bool flag = false;
    for(auto &i:word){
        if(isalpha(i)){
            flag = true;
        }else{
            return false;
        }
    }
    return flag;
}

void Dictionary::addDict(const string &word){
    for(auto &i :_dict){
        if(word == i._word){

            ++i._frequency;
            return;
        }
    }
    _dict.emplace_back(word, 1);
}

bool comp(Record &word1, Record &word2){
    if(word1._frequency != word2._frequency){
        return word1._frequency < word2._frequency; //按照频率做升序排序
    }else{
        return word1._word < word2._word;
    }
}

void Dictionary::read(const string &filename){
    ifstream ifs;
    ifs.open(filename);
    if(!ifs.good()){
        cerr<<"ifstream open file error\n";
        return;
    }    
    string word;
    _dict.reserve(4096);
    while(!ifs.eof()){
        ifs >> word;
        if(isword(word)){
            addDict(word);
        }                       
    }
    ifs.close();
    cout<<word<<endl;
}

void Dictionary::store(const string &filename){
    cout<<"store"<<endl;
    ofstream ofs;
    ofs.open(filename);
    if(!ofs.good()){
        cerr<<"ifstream open file error\n";
        return;
    }
    sort(_dict.begin(), _dict.end(), comp);
    for(auto &i : _dict){
        ofs << i._word <<'\t'<< i._frequency << endl;
    }
    ofs.close();
}


int main()
{
    Dictionary dict;
    dict.read("The_Holy_Bible.txt");
    dict.store("Total.txt");    
    return 0;
}

