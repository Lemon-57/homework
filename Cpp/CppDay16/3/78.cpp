#include<iostream>
#include<vector>
using namespace std;



class Solution {
public:
    vector<vector<int>> res;
    vector<int>path;
    
    vector<vector<int>> subsets(vector<int>& nums) {
        for(int i = 0; i < (int)nums.size(); i++){
            backtrack(nums, path, nums[i]);
        }
        return res;
    }
    void backtrack(vector<int>nums, vector<int>path, int start){
        res.push_back(path);
        for(int i = start; i < (int)nums.size(); i++){
            path.push_back(nums[i]);//路径选择
            backtrack(nums, path, i + 1);//进入下一条路
            path.pop_back();//删除最后一次路径(i+1)超出的
        }
    }
private:
};





int main()
{
    return 0;
}

