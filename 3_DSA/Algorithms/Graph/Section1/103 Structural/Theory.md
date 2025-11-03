> We will start Implementling BFS and DFS to solve problem.

Note : Try to visualize the code and dry run to understand each line.

- Cycle detection (directed)
- Cycle detection (undirected)
- Check tree
- connectivity
- bipartiteness (2-coloring via BFS)
- Topological sort (Kahn's algorithm)
- Topological sort (DFS approach)

> I will practice implement these then Move to solve CSES questions

- Counting Rooms 
- -
- till
- -
- Monsters


### Concepts

#### Cycle Detection (Undirected Graph)
- What it is:
  - A loop where you can start from a node and return without reusing an edge.
- Steps:
  1. Run DFS or BFS from each unvisited node.
  2. Track the parent node while moving.
  3. If you meet an already visited node that is not the parent, a cycle exists.

#### Cycle Detection (Directed Graph)
- What it is:
  - A loop that follows the direction of edges.
- Steps:
  1. Run DFS with three colors or visited states.
  2. Mark node as “visiting” when you enter.
  3. If you reach a “visiting” node again, the graph has a cycle.
  4. Mark node as “done” when you backtrack.

#### Check Tree
- What it is:
  - Decide if the graph is a tree (connected and no cycles).
- Steps:
  1. Check that there are exactly `n - 1` edges.
  2. Run DFS/BFS once; all nodes must be visited.
  3. Ensure no cycle is found during traversal.

#### Connectivity
- What it is:
  - All nodes can reach each other (one connected component).
- Steps:
  1. Pick any node and run DFS/BFS.
  2. Count how many nodes you visit.
  3. If count equals total nodes, the graph is connected.

#### Bipartite Check
- What it is:
  - Split nodes into two groups with no same-group edge.
- Steps:
  1. Run BFS/DFS and color the start node with color 0.
  2. Give opposite color to every neighbor.
  3. If you ever need to assign the same color to connected nodes, it is not bipartite.

#### Topological Sort using Kahn’s Algorithm
- What it is:
  - Linear order of nodes for DAGs where edges go from earlier to later.
- Steps:
  1. Compute in-degree for every node.
  2. Push nodes with in-degree 0 into a queue.
  3. Pop from queue, add to order, reduce in-degree of neighbors.
  4. When a neighbor’s in-degree becomes 0, push it into queue.

#### Topological Sort using DFS
- What it is:
  - Another way to list nodes so every edge goes from earlier to later.
- Steps:
  1. Run DFS on every unvisited node.
  2. After visiting all neighbors of a node, push it to a stack or list.
  3. Reverse the list at the end to get the topological order.

