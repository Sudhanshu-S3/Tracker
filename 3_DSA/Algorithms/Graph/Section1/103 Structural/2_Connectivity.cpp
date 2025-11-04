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




/*

# Connectivity
- What it is:
  - All nodes can reach each other (one connected component).
- Steps:
  1. Pick any node and run DFS/BFS.
  2. Count how many nodes you visit.
  3. If count equals total nodes, the graph is connected.

*/



/*

# Bipartite Check
- What it is:
  - Split nodes into two groups with no same-group edge.
- Steps:
  1. Run BFS/DFS and color the start node with color 0.
  2. Give opposite color to every neighbor.
  3. If you ever need to assign the same color to connected nodes, it is not bipartite.

*/