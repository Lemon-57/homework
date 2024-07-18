#include <iostream>
#include<vector>
using namespace std;

class Solution {
public:
    int search(vector<int>& arr, int target) {
        if(target == arr[0]){
            return 0;
        }
        int n = arr.size() - 1;
        int low = 0;
        int high = n;
        while(low <= high){
            int mid = (low + high)/2;
            if(target == arr[mid]){
                return mid;   
            }
            if(arr[0] <= arr[mid]){//左半边部分有序
                if(arr[0] < target && target < arr[mid]){
                    high = mid -1;
                }else{
                    low = mid + 1;
                }
            }
            else{ //if(arr[arr.size() -1] > arr[mid])//右半部分有序
                if(arr[mid] < target && target <=arr[n]){
                    low = mid + 1;
                }else{
                    high = mid -1;
                }
            }
        }
        return -1;
    }
};


int main()
{
    vector<int> arr{4, 5, 6, 7, 0, 1, 2};
    Solution st;
    int target;
    cin >> target;
    int result = st.search(arr, target);
    cout << result <<endl;
    return 0;
}

