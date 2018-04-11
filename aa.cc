#include<iostream>
#include<vector>
using namespace std;
bool Find(int target, vector<vector<int> > array) {
        if( array.size() == 0 || array[0].size() == 0)
            return false;
        int right = array.size() - 1;
        int top = 0;
        int  height = array.size() - 1;
        int width  = array[0].size() - 1;
        bool isFind = false;
        while(right >= 0 && top <= height){
            if(array[top][right] == target){
                isFind = true;
                break;
            }
            else if(array[top][right] > target)
                right--;
            else 
                top++;
        }
        return isFind;
    }

int main(){
    int n, m;
    int  target = -1;
    cin>>m;
    cin>>n;
    
    //vector<int> columnsN（columns）;
    vector<vector<int> > mat(m, vector<int>(n));
    int temp;
    for(int i = 0; i < m ; i++){
        for(int j = 0; j<n; j++)
            mat[i][j] = -1;
    }
    for(int i = 0; i < m ;i++){
        for(int j = 0; j < n; j++){
            if(cin>>temp)
                mat[i][j] = temp;
            else
                break;
        }
    }
	cin>>target;
        // cout<<"input error";
	if(Find(target, mat))
		cout<<1;
	else
        cout<<0;
}
