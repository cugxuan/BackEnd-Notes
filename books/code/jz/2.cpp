#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void replaceSpace(char *str, int length) {
        int i = 0, n = 0; //n为空格的数量
        while (i < length) {
            if (str[i++] == ' ')
                n++;
        }

        int Newlength = length + n * 2;  //n*3-n
        int j = length - 1;
        str[Newlength] = 0;
        for (i = Newlength - 1; i >= 0; i--, j--) {
            //i标记新的位置,j标记旧的位置
            if (str[j] == ' ') {
                str[i--] = '0';
                str[i--] = '2';
                str[i] = '%';
            } else {
                str[i] = str[j];
            }
        }
    }
};

int main() {
    Solution a;
    char *str = new char[100];
    strcpy(str, "a 123 bc   ahz");
    a.replaceSpace(str, 100);
    cout << str << endl;
}