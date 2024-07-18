#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <locale>//用以封装文化差异的多态刻面的集合

using namespace std;

string delete_spilth(string &doc){
    regex rx1("[，。][^，。]*占地[^，。]*[，。]");
    regex rx2("[。]占地[。]");

    string restr = "XXX第四五七医院，创建于1950年12月。位于武汉市江岸区工农兵路15号，占地15万平方米。";
    std::cout << delete_spilth(restr)<< std::endl;
    
    return regex_replace(doc, rx2, "");
}


int main()
{
    string str = "今天是个好日子圣达菲阿斯qweer";
    regex pattern("[\u4e00-\u9fa5]");
    sregex_token_iterator end;  //需要注意一下这里
    for (sregex_token_iterator j(str.begin(), str.end(), pattern); j != end; ++j){
        cout << *j;
    }
    cout << endl;
    return 0;
}

