#include<bits/stdc++.h>

using namespace std;

// 1. Cyclce detection in Undirected Graph.


bool CycleDetect(int node, int parent, vector<vector<int>>& adj , vector<int>& visited){

    visited[node] = true;

    for(auto u : adj[node]){

        if(!visited[u]){
            if(CycleDetect(u,node,adj,visited)) return true;
        }
        else if(u != parent )return true;
    }
    
    return false;
}

// Iterative approach of undirected graph

bool CycleDetect(int node, vector<vector<int>>& adj , vector<int>& visited){

    queue<pair<int,int>> q;

    q.push({node,-1});
    visited[node] = true;

    while(!q.empty()){
        auto current = q.front();
        q.pop();

        for(auto u:adj[current.first]){
            if(!visited[u]){
                visited[u] = true;
                q.push({u,current.first});
                
                
            }
            else if(u != current.second){return true;}
        }
    }

    return false;

}


// 2. Cycle Detection in Directed Graph


int main(){

    return 0;
}