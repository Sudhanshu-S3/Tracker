#include<bits/stdc++.h>
using namespace std;

//Frog II

/*

*/

// In Recursive DP is to store computation is done by recursion So.

vector<int> dpp;  // 1. Create
vector<int> h;

int frogJumpII(int i, int k) 
{
    if (i == 0) return 0;
    if (dpp[i] != -1) return dpp[i]; // 3. Use to stop
    int cost = INT_MAX;

    for (int j = 1; j <= k; ++j) 
    {
        if (i - j < 0) break;
        cost = min(cost, frogJumpII(i - j, k) + abs(h[i] - h[i - j]));
    }
    return dpp[i] = cost; // 2. Store
}

int main()
{   
    int n; cin>>n;
    vector<int> dp(n,0);
    int k;cin>>k;
    
    for (int i = 1; i < n; ++i) 
    {
        dp[i] = INT_MAX;
        for (int j = 1; j <= k; ++j) 
        {
            if (i - j < 0) break;
            dp[i] = min(dp[i], dp[i - j] + abs(h[i] - h[i - j]));
        }
    }
    cout << dp[n - 1];


    return 0;
}