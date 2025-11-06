#include <bits/stdc++.h>

using namespace std;


/*
=================
DP on DAG
=================

1. DP is Topological Relaxation

Previous states are not that well defined as before.

In DAG , Every edge (u -> v) means :
    to compute the best path to v , you may use the best path to u.

2. State:

dp [ v ] = length of the longest path ending at node v.

3. Base Cases:

dp [v] = 0 for the starting not because of indegree is 0;

4. Dependency order

So, for every node v, the possible predecessors are all usuch that u-> v is an edge

dp[v] = max ( dp [ u] +1);

*/

/*
Algorithm

Step   1 : 

Compute a topological order of nodes.

Step 2:
Traverse in topo order.

Step 3:
Answer = max(dp[v]);

*/

/*

Whenever you face a DAG-type problem, the template is:

for (node in topo_order)
    for (next in adj[node])
        dp[next] = combine(dp[next], transition(dp[node], node, next))


Change combine and transition based on problem type:

longest path → max(dp[next], dp[node] + weight)

shortest path → min(dp[next], dp[node] + weight)

counting paths → dp[next] += dp[node]

expected value → dp[next] = Σ(prob * dp[node])
That’s how tree DP, graph DP, and game DP all generalize from here.


*/

vector<vector<int>> adj;
vector<int> dp;

int dfs(int u) 
{
    if (dp[u] != -1) return dp[u];
    int best = 0;
    for (int v : adj[u])
    {
        best = max(best, 1 + dfs(v));
    }
    return dp[u] = best;
}

int main() {
    int n, m;
    cin >> n >> m;
    adj.assign(n, {});
    dp.assign(n, -1);

    for (int i = 0; i < m; ++i) 
    {
        int a, b; cin >> a >> b;
        adj[a-1].push_back(b-1);
    }

    int ans = 0;
    for (int i = 0; i < n; ++i)
    {
        ans = max(ans, dfs(i));
    }

    cout << ans;

    return 0;
}
