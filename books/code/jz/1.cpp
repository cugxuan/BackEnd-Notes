#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool Find(int target, vector<vector<int> > array) {
        int m = array.size();
        int n = array[0].size();
        int x = m - 1, y = 0;  //从左下角开始

        while (x >= 0 && y <= n - 1) {
            if (target < array[x][y]) {
                x--; //遇小往上移动
            } else if (target > array[x][y]) {
                y++; //遇大往右移动
            } else {
                return true;
            }
        }
        return false;
    }
};

int main() {
    vector<vector<int>> array;
    array.push_back(*(new vector<int>{1, 4, 9}));
    array.push_back(*(new vector<int>{4, 5, 10}));
    array.push_back(*(new vector<int>{7, 8, 15}));
    int i, j;
    for (i = 0; i < array.size(); i++) {
        for (j = 0; j < array[0].size() - 1; j++) {
            cout << array[i][j] << " ";
        }
        cout << array[i][j] << endl;
    }
    Solution a;
    cout << a.Find(2, array) << endl;
    cout << a.Find(6, array) << endl;
}