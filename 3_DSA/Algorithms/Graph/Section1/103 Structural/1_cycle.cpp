#include<bits/stdc++.h>

using namespace std;

// 1. a) Cyclce detection in Undirected Graph.


bool CycleDetect(int node, int parent, vector<vector<int>>& adj , vector<int>& visited)
{

    visited[node] = true;

    for(auto u : adj[node])
    {
        if(!visited[u])
        {
            if(CycleDetect(u,node,adj,visited)) return true;
        }
        else if(u != parent )return true;
    }
    
    return false;
}



// b) Iterative approach of undirected graph

bool CycleDetect(int node, vector<vector<int>>& adj , vector<int>& visited)
{

    queue<pair<int,int>> q;

    q.push({node,-1});
    visited[node] = true;

    while(!q.empty())
    {
        auto current = q.front();
        q.pop();

        for(auto u:adj[current.first])
        {
            if(!visited[u])
            {
                visited[u] = true;
                q.push({u,current.first});
            }
            else if(u != current.second)
            {
                return true;
            }
        }
    }

    return false;

}


// 2. a) Cycle Detection in Directed Graph

bool DirectedIterative (int node , vector<vector<int>>& adj , vector<int>& visited)
{
    stack<int> st;
    vector<bool> inStack(adj.size() , false);

    st.push(node);
    visited[node] = 1;
    inStack[node] = true;

    while(!st.empty())
    {
        int curr = st.top();
        bool pushedChild = false;

        for(auto u : adj[curr])
        {
            if(visited[u] == 0)
            {
                st.push(u);
                visited[u] = 1;
                inStack[u] = true;
                pushedChild = true;
                break;
            }
            else if(inStack[u])
            {
                return true;
            }
        }

        if ( !pushedChild)
        {
            st.pop();
            inStack[curr] = false;
            visited[curr] = 2;
        }

    }

    return false;
}

// b) Recursive Method of Cycle detection in Directed Graph

bool dfs(int node, vector<vector<int>>& adj , vector<int>& visited)
{

    visited[node] = 1;

    for(auto u : adj[node])
    {
        if(visited[u] == 0)
        {
            if(dfs(u,adj,visited)) return true;
        }
        else if(visited[u] == 1)
        {
            return true;
        }

    }
    visited[node] = 2;
    return false;
}

// c) we can detect cycle using Kahn's (Topological Sorting)
/*
Idea:

- Compute in-degrees of all nodes.
- Push all nodes with indegree == 0 into queue.
- Pop nodes and decrement indegree of neighbors.
- If processed node count < total nodes → cycle exists.
*/

int main()
{
    cout << "=== UNDIRECTED GRAPH TEST ===\n";

    // Example Undirected Graph with cycle:
    // 0 - 1 - 2 - 3
    //      \_____/
    vector<vector<int>> adj1 = 
    {
        {1},        // 0
        {0, 2},     // 1
        {1, 3},     // 2
        {2, 1}      // 3 (cycle back to 1)
    };

    vector<int> visited1(4, 0);
    if(CycleDetect(0, -1, adj1, visited1))
        cout << "DFS Undirected: Cycle detected \n";
    else
        cout << "DFS Undirected: No cycle \n";

    vector<int> visited1b(4, 0);

    if(CycleDetect(0, adj1, visited1b))
        cout << "BFS Undirected: Cycle detected \n";
    else
        cout << "BFS Undirected: No cycle \n";


    cout << "\n=== DIRECTED GRAPH TEST ===\n";

    // Example Directed Graph with a cycle:
    // 0 → 1 → 2
    //      ↑   |
    //      └───┘
    vector<vector<int>> adj2 = 
    {
        {1},    // 0
        {2},    // 1
        {1}     // 2 (cycle back to 1)
    };

    vector<int> visited2(3, 0);

    if(DirectedIterative(0, adj2, visited2))
        cout << "Iterative Directed: Cycle detected \n";
    else
        cout << "Iterative Directed: No cycle \n";

    vector<int> visited2b(3, 0);
    if(dfs(0, adj2, visited2b))
        cout << "Recursive Directed: Cycle detected \n";
    else
        cout << "Recursive Directed: No cycle \n";


    return 0;
}