#include<bits/stdc++.h>
using namespace std;


// Link : https://atcoder.jp/contests/dp/tasks/dp_c

vector<vector<int>> dp;
vector<vector<int>> store;

int vacation(int i , int k)
{
    if(i == 0) return store[0][k];

    if(dp[i][k] != -1) return dp[i][k];

    int happy = INT_MIN;

    for(int j = 0; j<3 ; j++)
    {
        if(j == k) continue;
        happy = max(happy ,store[i][j] + vacation(i-1 , j) ) ;
    }
    
    return dp[i][k] = happy;
}

int main()
{
    int N; cin>>N;
    for(int i= 0 ; i< N ; i++)
    {
        for(int j = 0 ; j<3 ; j++)
        {
            cin>>store[i][j];   
        }
    }
    dp.assign(N, vector<int>(3, -1));

    int res = INT_MIN;

    for(int i=0 ; i< 3 ; i++)
    {
        res = max(res , vacation(N-1, i));
    }
    return 0;
}