class Solution {
public:
    bool Find(int target, vector<vector<int> > array) {
        int m=array.size();
        int n=array[0].size();
        int x=m-1,y=0;  //从左下角开始

        while(x>=0&&y<=n-1){
            if(target<array[x][y]){
                x--; //遇小往上移动
            }else if(target>array[x][y]){
                y++; //遇大往右移动
            }else{
                return true;
            }
        }
        return false;
    }
};
