#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void replaceSpace(char *str, int length) {
        int i = 0, spaceNum = 0; //spaceNum为空格的数量
        int strl = strlen(str); // 字符串的长度
        while (i < strl) {
            if (str[i++] == ' ')
                spaceNum++;
        }

        // 计算需要的空间，如果超出空间则返回
        int Newlength = strl + spaceNum * 2;  //n*3-n
        if (Newlength > length) {
            return;
        }
        int j = strl - 1;
        str[Newlength] = '\0';
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