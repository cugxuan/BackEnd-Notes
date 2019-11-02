#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void MoreThan3ofNum_Solution(vector<int> numbers) {
        if (numbers.size() <= 0) {
            return;
        }

        int a = 0, b = 1;
        int ca = 0, cb = 0;
        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] == a) {
                ca++;
            } else if (numbers[i] == b) {
                cb++;
            } else if (ca == 0) {
                a = numbers[i];
                ca++;
            } else if (cb == 0) {
                b = numbers[i];
                cb++;
            } else {
                ca--;
                cb--;
            }
        }
        ca = cb = 0;
        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] == a)
                ca++;
            if (numbers[i] == b)
                cb++;
        }
        if (ca * 3 > numbers.size())
            cout << a << endl;
        if (cb * 3 > numbers.size())
            cout << b << endl;
    }

};

int main() {
    vector<vector<int>> a;
    a.push_back(vector<int>{1, 2, 3, 2, 3, 2, 2, 3, 3}); // 2 和 3
    a.push_back(vector<int>{1, 1, 2, 3}); // 1
    a.push_back(vector<int>{1, 2, 3, 3}); // 3
    a.push_back(vector<int>{1, 3, 3, 3}); // 3
    a.push_back(vector<int>{1, 2, 1, 3, 3, 2, 4, 5}); // 无
    a.push_back(vector<int>{1, 1, 1, 1, 2, 2, 4, 5, 6, 7, 8}); // 1

    Solution test;
    for (int i = 0; i < a.size(); i++) {
        cout << "case" << i << endl;
        test.MoreThan3ofNum_Solution(a[i]);
    }
}