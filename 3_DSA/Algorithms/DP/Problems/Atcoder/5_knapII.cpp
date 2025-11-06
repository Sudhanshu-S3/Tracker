#include <bits/stdc++.h>
using namespace std;


// Link :https://atcoder.jp/contests/dp/tasks/dp_e

const long long INF = 1e15;
vector<long long> wt, val;
vector<vector<long long>> dp;

long long knap2(int i, int v) {
    if (v == 0) return 0;        // zero value ⇒ zero weight
    if (i < 0) return INF;       // no items left
    if (dp[i][v] != -1) return dp[i][v];

    long long res = knap2(i - 1, v);  // skip item i

    if (v >= val[i]) {
        res = min(res, wt[i] + knap2(i - 1, v - val[i]));
    }

    return dp[i][v] = res;
}

int main() {
    int n;
    long long W;
    cin >> n >> W;
    wt.resize(n);
    val.resize(n);

    long long sumV = 0;
    for (int i = 0; i < n; ++i) {
        cin >> wt[i] >> val[i];
        sumV += val[i];
    }

    dp.assign(n, vector<long long>(sumV + 1, -1));

    long long ans = 0;
    for (int v = 0; v <= sumV; ++v) {
        if (knap2(n - 1, v) <= W)
            ans = v;
    }
    cout << ans << "\n";
}
