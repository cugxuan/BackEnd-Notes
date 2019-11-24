#include <bits/stdc++.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

//------------answer-----------------
class Solution {
public:
    TreeNode *reConstructBinaryTree(vector<int> pre, vector<int> vin) {
        //注意结果是长度减1
        TreeNode *root = reConstructBinaryTree(pre, vin, 0, pre.size() - 1, 0, vin.size() - 1);
        return root;
    }

private:
    TreeNode *
    reConstructBinaryTree(vector<int> &pre, vector<int> &vin, int startPre, int endPre, int startIn, int endIn) {
        //如果超出则返回
        if (startPre > endPre || startIn > endIn)
            return NULL;
        TreeNode *root = new TreeNode(pre[startPre]);

        for (int i = startIn; i <= endIn; i++) {
            if (pre[startPre] == vin[i]) {
                //关键是计算前序和中序开始的位置
                root->left = reConstructBinaryTree(pre, vin, startPre + 1, startPre + i - startIn, startIn, i - 1);
                root->right = reConstructBinaryTree(pre, vin, i - startIn + startPre + 1, endPre, i + 1, endIn);
            }
        }
        return root;
    }
};
//------------answer-----------------

void Print(TreeNode *pos) {//中序输出
    if (pos->left != NULL)
        Print(pos->left);
    cout << pos->val << " ";
    if (pos->right != NULL)
        Print(pos->right);
}

int main() {
    int a[] = {1, 2, 4, 7, 3, 5, 6, 8};
    int b[] = {4, 7, 2, 1, 5, 3, 8, 6};
    vector<int> pre(8);
    vector<int> vin(8);
    for (int i = 0; i < 8; i++)
        pre[i] = a[i];
    for (int i = 0; i < 8; i++)
        vin[i] = b[i];
    Solution test;
    TreeNode *root = test.reConstructBinaryTree(pre, vin);
    Print(root);
}