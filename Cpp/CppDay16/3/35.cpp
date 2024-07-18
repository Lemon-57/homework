#include <iostream>
#include<vector>
using namespace std;
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int start = 0;
        int end = nums.size() - 1;
        int mid;
        while(start <= end){
            mid = (start + end)/2;
            if(target < nums[mid]){
                end = mid - 1;
            }
            else if(target > nums[mid])
            {
                start = mid + 1;
            }
            else{
                cout << mid << endl;
                return mid;
            }
        }
        for(int i = 0; i < (int)nums.size(); i++){
            if(nums[i] > target){
                cout << i << endl;
                return i;
            }
        }
        return nums.size();
    }
};
int main()
{
    Solution st;
    vector<int>nums = {1, 2, 4, 5, 6};
//    st.searchInsert(nums, 5);
    st.searchInsert(nums, 3);
    return 0;
}

