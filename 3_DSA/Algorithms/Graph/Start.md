# Graph Algorithms — Roadmap & Structure

[graphMindmap](/Algorithms/Graph/graph%20mindmap.png)
---

## Section 1 — Foundations: Traversal & Basic Shortest Paths

Goal: Understand representations, traversals, and simple pathfinding.
Target : Able to implement Basic graph operation.

### 1. Graph Basics
- Representations: adjacency list, adjacency matrix, edge list
- Types: directed vs undirected, weighted vs unweighted, cyclic vs acyclic

### 2. Traversal
- DFS — recursive and iterative (stack)
- BFS — queue-based
- Connected components (count via DFS/BFS)

### 3. Simple Structural Problems
- Cycle detection (directed & undirected)
- Check tree, connectivity, bipartiteness (2-coloring via BFS)
- Topological sort (Kahn's algorithm and DFS approach)

### 4. Basic Shortest Path
- BFS for shortest paths in unweighted graphs
- Dijkstra (non-negative weights)
- Reconstructing/printing a shortest path
- Complexity trade-offs: adjacency list vs matrix

Practice milestones:
- Clean DFS/BFS implementations
- Topological sort of DAGs
- Example problems: shortest path in an unweighted maze, bipartite check.

---

## Section 2 — Intermediate: Variants & Structural Algorithms

Goal: Move to weighted variants, SCCs, MSTs, and DSU.

### 5. Shortest Path Variants
- Bellman–Ford (negative weights)
- Floyd–Warshall (all-pairs)
- Shortest path on DAG (topo + DP)
- K-shortest paths, limited-edge shortest paths

### 6. Strongly Connected Components & DAG
- Longest/shortest path via DP on DAGs
- Counting paths with memoization
- Kosaraju, Tarjan
- Condensation graph (SCC DAG)

### 7. Bridges & Articulation Points
- Find bridges and articulation vertices
- Applications: critical connections

### 8. Minimum Spanning Tree & DSU
- Kruskal (with Union-Find), Prim, Borůvka
- DSU: union by rank/size, path compression, rollback

Practice milestones:
- Implement Bellman-Ford and negative cycle detection
- Compute SCCs, bridges, MSTs
- Example problems: Cheapest Flights Within K Stops, Redundant Connection, Accounts Merge

---

## Section 3 — Advanced: Optimization & Hybrid Problems

Goal: Combine graph concepts with DP, decomposition, and real-world constraints.

### 9. Decomposition & Connectivity
- Biconnected components, block-cut trees
- Eulerian trails (Hierholzer)
- Heavy-Light & centroid decomposition

### 10. Matching, Coloring & Scheduling
- Greedy coloring, bipartite matching (Hopcroft–Karp)
- Edge coloring, scheduling reductions

### 11. Advanced DAG & DP
- Bitmask DP & TSP variants

### 12. Transformations & Flow
- Graph complements, isomorphism heuristics
- A* search, introduction to max-flow/min-cut

Practice milestones:
- Scale algorithms to large limits (N ~ 1e5, E ~ 1e6)
- Combine techniques: DP on SCC DAG, DSU rollback
- Example problems: Network Delay Time variants, Reconstruct Itinerary, Critical Connections

---


## Resources
- Guide to CP (theory): https://duoblogger.github.io/assets/pdf/memonvyftw/guide-t-cp.pdf
- Problem sets: CSES — https://cses.fi/problemset/



> NOTE: I will add topic-wise good questions as I solve them.
