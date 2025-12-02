#include<bits/stdc++.h>

using namespace std;
/*

#Check Tree
- What it is:
  - Decide if the graph is a tree (connected and no cycles).
- Steps:
  1. Check that there are exactly `n - 1` edges.
  2. Run DFS/BFS once; all nodes must be visited.
  3. Ensure no cycle is found during traversal.

*/

bool isTree( int n , int edgesCount , const vector<vector<int>>& adj )
{
  if ( edgesCount != n-1 ) return false;
  if ( n == 0 ) return true;

  vector <char> visited( n , 0 );
  queue  <int>  q;

  q.push(0);
  visited[0] = 1;
  int cnt = 1;
  while (!q.empty())
  {
    int u = q.front();
    q.pop();

    for ( int v : adj[u] )
    {
      if(!visited[v])
      {
        visited[v] = 1;
        q.push( v );
        ++cnt;
      }
    }
  }
  return cnt == n;
}


/*

# Connectivity
- What it is:
  - All nodes can reach each other (one connected component).
- Steps:
  1. Pick any node and run DFS/BFS.
  2. Count how many nodes you visit.
  3. If count equals total nodes, the graph is connected.

*/

bool isConnected ( int n , const vector<vector<int>>& adj)
{
  if ( n == 0 ) return true;
  vector <char> visited(n,0);
  queue  <int>  q;

  q.push( 0 );
  visited[0] = 1;
  int cnt = 1;

  while (!q.empty())
  {
    int u = q.front(); 
    q.pop();

    for ( int v : adj[u])
    {
      if (!visited[v])
      {
        visited[v] = 1;
        q.push( v );
        ++cnt;
      }
    }
  }
  return cnt == n;
}

/*

# Bipartite Check
- What it is:
  - Split nodes into two groups with no same-group edge.
- Steps:
  1. Run BFS/DFS and color the start node with color 0.
  2. Give opposite color to every neighbor.
  3. If you ever need to assign the same color to connected nodes, it is not bipartite.

*/

bool isBipartite ( int n , const vector<vector<int>>& adj )
{
  vector <int> color( n , -1 );

  for ( int i =0 ; i < n ; ++i )
  {
    if ( color[i] != -1 ) continue;

    queue<int> q;
    q.push( i );

    color[i] = 0;

    while ( !q.empty())
    {
      int u = q.front();
      q.pop();
      for( int v : adj[u])
      {
        if( color[v] == -1 )
        {
          color[v] = 1 - color[u];
          q.push( v );
        }
        else if (color[v] == color[u])
        {
          return false;
        }
      }
    }
  }

  return true;
}