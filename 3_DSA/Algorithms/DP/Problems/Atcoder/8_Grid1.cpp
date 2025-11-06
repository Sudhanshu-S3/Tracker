#include<bits/stdc++.h>
using namespace std;


// Link : https://atcoder.jp/contests/dp/tasks/dp_h

int MOD = 1e9;

vector<vector<int>> dp;
vector<vector<char>> store;

int grid1(int x, int y)
{
    if(x < 0 ||  y < 0 ) return 0;
    if(store[x][y] == '#') return 0;
    if(x==0 && y == 0) return 1;

    if(dp[x][y] != -1) return dp[x][y];

    int ways = 0;
    if (x > 0) ways = (ways + grid1(x - 1, y)) % MOD;  // From above
    if (y > 0) ways = (ways + grid1(x, y - 1)) % MOD;  // From left

    return dp[x][y] = ways;
}

int main()
{
    int n,m;
    cin>>n>>m;

    store.resize(n , vector<char> (m));
    dp.resize(n,vector<int> (m,-1));

    for(int i = 0 ; i<n ; i++)
    {
        for(int j = 0; j < m ; j++)
        {
            cin>>store[i][j];
        }
    }

    

    cout<<grid1(n-1, m-1)<<endl;
    return 0;
}