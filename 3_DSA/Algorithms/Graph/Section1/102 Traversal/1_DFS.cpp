#include<bits/stdc++.h>

using namespace std;


/*

- 1. DFS — recursive and iterative (stack)
- 2. BFS — queue-based
- 3. Connected components (count via DFS/BFS)

*/

// 1. DFS 

//Here i will implement both the recusive DFS and the Iterative DFS


// Recursive DFS 

// STEPs : 

/*
1. Think about : we have a start node.
2. we know that it not visited so mark visited. and print the node.
3. Checking its neighbors and only push those who are not visited.
4. Call the DFS again it rerun the above steps.

*/

void DFSre(int node, vector<vector<int>>& adj , vector<bool>& visited){

    visited[node] = true;

    cout<<node<<" ";

    for(auto u: adj[node]){

        if(!visited[u]){
            
            DFSre(u, adj, visited);
        }
    }
}


// Iterative DFS using STACK


void DFSit(int node, vector<vector<int>>& graph ){


    //1. Step 1 : Creating Stack & visited Array.

    stack<int> st;
    vector<bool> visited(node, false);

    // 2. Step 2 : Inserting the First node is stack;

    st.push(node);

    while(!st.empty()){
        int current = st.top();
        st.pop();

        if(!visited[current]){

            visited[current] = true; // Mark Visited
            cout<<current<<" ";

            // 3. Checking neighbors which are not visited.
            for(auto u : graph[current]){

                if( !visited[u] ){
                    visited[u] = true;
                    st.push(u);
                }
            }
        }
    }
}


// Recursive DFS Extra Practice :

// 1. Undirected graph with Adjacency list.
// 2. Directed Graph with Adj List.
// 3. graph with Adj Matrix.
// 4. Disconnected Graph.


// 1. Undirected graph with Adj list
void DFSug(int node , vector<vector<int>>& adj , vector<int>& visited){

    visited[node] = true;
    cout<<node<<" ";

    for(auto u : adj[node]){

        if(!visited[u]){
            DFSug(u,adj,visited);
        }
    }

}

//  2. Direct graph with Adj List

//Code would be same the adj list make all difference.

void DFSdg(int node, vector<vector<int>>& adj , vector<int>& visited){

    visited[node] = true;
    cout<<node<<" ";

    for(auto u: adj[node]){

        if(!visited[u]){
            DFSdg(u, adj, visited);
        }
    }
}

// 3. Graph with adj matrix

void DFSam(int startNode, vector<vector<int>>& adjMatrix,vector<int>& visited , int Nodes){
    
    //1. Mark the current Node as Visited
    visited[startNode] = true;
    cout<<startNode<<" ";

    for(int i=0 ; i <Nodes ; i++){
        // 2. Check for Neighbor or not , and then Unvisited Neighbor or not.
        if(!visited[i] && adjMatrix[startNode][i] == 1){
            DFSam(i, adjMatrix, visited, Nodes);
        }
    }
}

// 4.  Disconnect Graph 

// DFS function will remain unchange.

/*
int disconnect components , count = 0;

for(int i= 0 ;i < Nodes ; i++){

    if(visited[i] == false){
        count++;
        dfs( i , adj, visited );
    }

}

*/

int main(){

    return 0;
}