#include<bits/stdc++.h>

using namespace std;

//BFS Iterative approach using Queue

void BFS(int node, vector<vector<int>>& adj , vector<int>& visited){
    queue<int> q;
    visited[node] = true;
    q.push(node);

    while(!q.empty()){
        int current = q.front();
        q.pop();
   
        cout<< current<< " ";

            for(auto u : adj[current]){
                
                if(!visited[u]){
                    visited[u] = true;
                    q.push(u);
                }
            }
    }
}
