#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    /**
     * 解法一：
     * 排序后从中间找看数量是否超过一半
     * 时间复杂度O(nlogn)+O(n)
     */
    int MoreThanHalfNum_Solution_QSort(vector<int> numbers) {
        int n = numbers.size();
        sort(numbers.begin(), numbers.end());
        int num = numbers[n / 2], ans = 1;
        int pos = n / 2 + 1;
        while (numbers[pos++] == num)
            ans++;
        pos = n / 2 - 1;
        while (numbers[pos--] == num)
            ans++;
        if (ans * 2 > n)
            return numbers[n / 2];
        else
            return 0;
    }

    /**
     * 解法二：
     * 摘自用户 [cm问前程](https://www.nowcoder.com/profile/429784)
     * 采用阵地攻守的思想：
     * 第一个数字作为第一个士兵，守阵地；count = 1；
     * 遇到相同元素，count++;
     * 遇到不相同元素，即为敌人，同归于尽,count--；当遇到count为0的情况，又以新的i值作为守阵地的士兵，继续下去，到最后还留在阵地上的士兵，有可能是主元素。
     * 再加一次循环，记录这个士兵的个数看是否大于数组一般即可。
     */
    int MoreThanHalfNum_Solution(vector<int> numbers) {
        if (numbers.size() <= 0) {
            return 0;
        }

        int length = numbers.size();
        int result = numbers[0];
        int times = 1;
        for (int i = 0; i < length; i++) {
            if (times == 0) {
                result = numbers[i];
                times = 1;
            } else if (numbers[i] == result)
                times++;
            else
                times--;
        }
        // 判断 result 是否超过一半
        times = 0;
        for (int i = 1; i < length; i++) {
            if (numbers[i] == result)
                times++;
        }
        if (times * 2 > length) {
            return result;
        }
        return 0;
    }

};

int main() {
    vector<int> a{1, 2, 3, 2, 4, 2, 5, 2, 3};
    Solution test;
    cout << test.MoreThanHalfNum_Solution(a) << endl;
    cout << test.MoreThanHalfNum_Solution_QSort(a) << endl;
}