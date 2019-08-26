#include <bits/stdc++.h>

using namespace std;

class Solution {
//    实现?int sqrt(int x)?函数。
//    计算并返回?x?的平方根，其中?x 是非负整数。
//    由于返回类型是整数，结果只保留整数的部分，小数部分将被舍去。
public:
//    double 类型算出来开平方的精确值
    int mySqrt0(int x) {
        if (x == 1)
            return 1;
        double left = 0, right = x, mid;
        while (1) {
            mid = (left + right) / 2;
            if ((mid * mid - x) > 1e-5) {
                right = mid;
            } else if ((mid * mid - x) < -1e-5) {
                left = mid;
            } else {
                break;
            }
        }
        return int(mid);
    }
// 二分法
    int mySqrt1(int x) {
        //注：在中间过程计算平方的时候可能出现溢出，所以用long long。
        //对于一个非负数n，它的平方根不会大于（n/2+1）
        long long left = 0, right = x / 2 + 1;
        long long mid, res;
        while (left <= right) {
            mid = (left + right) / 2;
            res = mid * mid;
            if (res == x)
                return mid;
            else if (res < x)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return right;
    }
//    牛顿迭代法
//    https://www.zhihu.com/question/20690553/answer/146104283
    int mySqrt(int x) {
        if (x == 0)
            return 0;
        long long cur = x;
        // 因为这里是求平方，只需要算到小就可以退出了
        while (cur * cur > x) {
            cur = (cur * cur + x) / (2 * cur);
        }
        return floor(cur);
    }

};

int main() {
    // freopen("../input.txt", "r", stdin);
    int n;
    Solution a;
    while (cin >> n) {
        cout << a.mySqrt(n) << endl;
    }
}