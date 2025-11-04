## Basic Shortest Path Algorithms

Know when to use BFS, Dijkstra, or other tools based on edge weights and graph size.

### General Checklist

1. Confirm if the graph is directed or undirected.
2. Check edge weights:
   - Use **BFS** if all edges have the same cost (often 1).
   - Use **Dijkstra** when weights are non-negative but different.
3. Choose an adjacency list for sparse graphs; adjacency matrix only if the graph is tiny.
4. Keep a parent array to rebuild the path when needed.
5. Test with small graphs to ensure distances update correctly.


## Concepts

---

### BFS (Unweighted Shortest Path)

- **When**: edges all cost the same (0/1 after conversion to layers).
- **Idea**: layered exploration—first time you reach a node is the shortest path.
- **Steps**:
  1. Push the start node into a queue with distance `0`.
  2. Pop the current node, visit each neighbor not yet seen.
  3. Set `dist[neighbor] = dist[current] + 1` and record parent.
  4. Continue until the queue is empty or the target is found.


### Dijkstra’s Algorithm

- **When**: positive or zero weights, no negative edges.
- **Idea**: greedy expansion using a min-priority queue.
- **Steps**:
  1. Initialize all distances to `INF`, except start node `0`.
  2. Push `(0, start)` into a min-heap.
  3. Pop the node with smallest distance. If it is stale, skip.
  4. For each neighbor, relax:  
     `if dist[u] + w < dist[v]`, update and push `(dist[v], v)` into heap.
  5. Repeat until heap is empty or the target distance is final.


### Reconstructing a Path

- **Goal**: print the actual path, not only distance.
- **Steps**:
  1. Keep `parent[v] = u` whenever you update `dist[v]`.
  2. After the search, start from the destination node and walk back using parents.
  3. Reverse the collected list to get the path from start to end.
  4. Handle “no path” by checking if distance stayed `INF`.


### Complexity Notes

- **Adjacency list + BFS**: `O(n + m)`.
- **Adjacency list + Dijkstra (binary heap)**: `O(m log n)`.
- **Adjacency matrix**: simplifies code but costs `O(n^2)` per run, so only use for dense or tiny graphs.

---


### Practice Flow

1. Implement BFS shortest path on a grid or unweighted graph.
2. Code Dijkstra with `std::priority_queue`.
3. Add parent tracking and print the shortest path.
4. Compare performance with adjacency list vs matrix on the same input.
5. Take notes on edge cases (disconnected nodes, multiple components).