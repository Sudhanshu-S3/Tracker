#include<bits/stdc++.h>

using namespace std;


// Link :  https://atcoder.jp/contests/dp/tasks/dp_d


/*
dp[i][w] = the weight avaiable to store and the current item chosen

*/

vector<vector<int>> dp;
vector<int> weights, value;



int knap(int i , int W)
{
    if(i < 0 ) return 0;
    if(dp[i][W] != -1) return dp[i][W];

    int not_take = knap(i-1, W);

    int take = 0;

    if(weights[i] <= W)
    {
        take = value[i] + knap( i-1 , W-weights[i] );
    }
    return dp[i][W]= max(take , not_take);
} 


int main()
{
    int n; int W;
    cin >> n >> W;
    weights.resize(n); 
    value.resize(n);

    for (int i = 0; i < n; ++i) cin >> weights[i] >> value[i];

    dp.assign(n, vector<int>(W + 1, -1));
    
    cout << knap(n - 1, W) << "\n";
    return 0;
}