#include <iostream>
#include<vector>
#include<string>
#include<unordered_set>
#include<deque>
using std::vector;
using std::string;
using std::unordered_set;
using std::deque;
// 定义 层节点 容器
// 根节点 压入容器内
// while (容器非空) {
//     获取容器内节点的个数（即当前处理层节点个数）
//     for (当前层节点个数) {
//         从容器中获取一个节点
//         按业务逻辑处理单个节点
//         获取当前节点的 子节点 压入容器内
//     }
// }

//每一个单词是一个节点，仅有单个字符不同的两个节点之间视为连通，
//我们需要找到从起点到终点最短的路径并输出
class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string>wordDict(wordList.begin(), wordList.end());//hash 提高查找速度O(1)
        unordered_set<string>beginDict{beginWord};
        unordered_set<string>endDict{endWord};
        vector<vector<string>>res;
        deque<vector<string>>worker;//层节点容器 //deque 如果只需要在头部进行插入和删除
        worker.push_back({beginWord});//beginWord 作为起始的根节点
        while(!wordDict.empty()){
            unordered_set<string>visited;//获取当前容器内的节点个数
            for(auto word : beginDict){
                beginDict.erase(word);
            }
            for(auto word : beginDict){
                for(int i = 0; i < word.size(); i++){
                    string str = word;
                    int j = 0;
                    for(char ch = 'a'; ch < 'z'; ch++){
                        str[i] = ch;
                        auto it = wordDict.find(str);
                        if(wordDict.end() == it){
                            continue;
                        }
                        else{//如果满足
                            res[j].push_back(*it);
                            j++;
                            visited.insert(*it);
                        }
                    }
                }
            }
            
        }

    }
};



int main()
{
    string beginWord = "hit", endWord = "cog";
    vector<string> wordList = {"hot","dot","dog","lot","log","cog"};
    Solution st;
    st.findLadders( beginWord,  endWord, wordList);

    return 0;
}


