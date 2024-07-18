#include <iostream>
#include<string>
#include<vector>
#include<unordered_set>
using namespace std;


class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // 将vector转化成哈希表方便后续操作 由于不用考虑个数 且元素总数量未知 因此用unordered_set
        unordered_set<string>wordDict(wordList.begin(), wordList.end());
        //先验证wordDict中是否有含有endWord
        if(wordDict.end() == wordDict.find(endWord) ){
            return 0;
        }
        unordered_set<string>beginDict{beginWord};
        unordered_set<string>endDict{endWord};
        int step = 1;
        while (!beginDict.empty())
        {
            ++step; 
            unordered_set<string>temp;//存储这次找到的路径单词
            for(auto word : beginDict){//将使用过的单词从wordDict中删除，不然就是重复路径
                wordDict.erase(word);
            }
            for(auto word : beginDict){//根据头来找单词
                for(int i = 0; i < word.size(); i++){//遍历每一位
                    string str = word;
                    for(char ch = 'a'; ch <= 'z'; ch++){//变换每一位的字符和剩下wordDict中的单词进行比较
                        str[i] = ch;
                        if(wordDict.end() == wordDict.find(str)){//不存在不执行后面的，直接进行下一次单词
                            continue;
                        }else{//如果存在再判断它是不是在endDict中
                            if(endDict.end() != endDict.find(str)){
                                return step; //如果存在endDict中代表，已经走到endWord,路径完成
                            }else{//没走到末尾，将本次路径单词记录到temp中
                                temp.insert(str);
                            }
                        // }
                    }
                }
            }
            //根据temp的规模来判断是否更新头
            //如果temp的规模比endDict大，下一次单层搜索任务就交给endDict 否则继续由beginDict执行
            if(temp.size() > endDict.size()){
                beginDict = endDict;//将上次的路径单词赋给beginDict 以便再剩于wordDict中查找能和路径连接起来的单词
                endDict = temp;//将本次路径赋给赋给endict
            }else{            
                beginDict = temp;
            }
        }
        return 0;
    }
    }
};


int main()
{
    string beginWord = "hit", endWord = "cog";
    vector<string> wordList = {"hot","dot","dog","lot","log","cog"};
    Solution st;
    st.ladderLength( beginWord,  endWord, wordList);
    return 0;
}

