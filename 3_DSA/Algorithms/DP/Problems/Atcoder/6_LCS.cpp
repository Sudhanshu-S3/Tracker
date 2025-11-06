#include<bits/stdc++.h>
using namespace std;

// Link : https://atcoder.jp/contests/dp/tasks/dp_f

vector<vector<int>> dp;
string s,t;

int LCS(int a, int b )
{
    if(a<0 || b<0) return 0 ;

    if(dp[a][b] != -1) return 0;

    int curr;

    if(s[a] == t[b])
    {
        curr = 1 + LCS(a-1, b-1) ;
    }
    else if( s[a] == t[b])
    {
        curr= max(LCS(a-1, b), LCS(a,b-1));
    }

    return dp[a][b] = curr;
}

int main()
{
    cin >> s >> t;
    int n = s.size(), m = t.size();

    dp.assign(n, vector<int>(m, -1));

    cout << LCS(n - 1, m - 1);
    return 0;    
}