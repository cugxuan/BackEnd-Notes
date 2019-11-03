#include <bits/stdc++.h>

using namespace std;
/**
*  struct ListNode {
*        int val;
*        struct ListNode *next;
*        ListNode(int x) :
*              val(x), next(NULL) {
*        }
*  };
*/
class Solution {
public:
    //--------递归方法，实际中可能爆栈----------
    vector<int> vec;
    void explore(ListNode* head){
        if(head!=NULL){
            printListFromTailToHead(head->next);
            vec.push_back(head->val);
        }
    }
    vector<int> printListFromTailToHeadRe(ListNode* head) {
        explore(head);
        return vec;
    }
    //----------使用栈先进后出---------------
//    vector<int> vec;
    stack<int> a;
    vector<int> printListFromTailToHeadStack(ListNode* head) {
        //把val依次入栈
        while(head!=NULL){
            a.push(head->val);
            head=head->next;
        }
        //栈先进后出，放到vec
        while(!a.empty()){
            vec.push_back(a.top());
            a.pop();
        }
        //返回vec
        return vec;
    }
    //----------使用stl的reverse函数-------------
//    vector<int> vec;
    vector<int> printListFromTailToHead(ListNode* head) {
        while(head!=NULL){
            vec.push_back(head->val);
            head=head->next;
        }
        reverse(vec.begin(),vec.end());
        return vec;
    }
};