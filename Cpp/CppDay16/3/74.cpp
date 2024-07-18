#include <iostream>
#include<vector>
using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target){
        int lineSize = matrix.size(); //几行
        int arrSize = matrix[0].size();//几列
        for(int i = 0; i < lineSize; i++){
            for(int j = 0; j < arrSize; j++){
                if(matrix[i][j] ==  target){
                    return true;
                }
            }
        }
        return false;
    }
};

int main()
{
    vector<vector<int>> matrix = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
    return 0;
}

