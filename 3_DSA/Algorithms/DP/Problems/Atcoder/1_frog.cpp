#include<bits/stdc++.h>
using namespace std;

/*
dp[i] = minimum cost to get at index i stone
*/

vector<int> dp;

int frogJump(int i, const vector<int>& h) {

    if (i == 0) return 0;
    if (dp[i] != -1) return dp[i];
    int cost1 = frogJump(i - 1, h) + abs(h[i] - h[i - 1]);
    int cost2 = INT_MAX;
    if (i > 1) cost2 = frogJump(i - 2, h) + abs(h[i] - h[i - 2]);
    return dp[i] = min(cost1, cost2);
    
}

// b. Iterative Solution

int iterativefrog(int N , vector<int>& heights)
{
    int prev2 = 0;
    int prev1 = abs(heights[1] - heights[0]);
    for (int i = 2; i < N; ++i) 
    {
        int curr = min(
            prev1 + abs(heights[i] - heights[i - 1]),
            prev2 + abs(heights[i] - heights[i - 2])
        );
        prev2 = prev1;
        prev1 = curr;
    }
    cout << prev1;

}


int main()
{

    return 0;
}