#include<bits/stdc++.h>

using namespace std;


/*
1. Graph Basics
- Representations: adjacency list, adjacency matrix, edge list
- Types: directed vs undirected, weighted vs unweighted, cyclic vs acyclic

- I will try to implement One example of each type for practice. 
*/

/* GRAPH REPRESENTATION */

// 1.The Adjacency Matrix Implementation of Graph

class Graph {

    private:

        int nodes, edges;

        vector<vector<int>> graph;


    public:

        Graph(){

            cin>> nodes >> edges;

            graph.resize(nodes, vector<int> (nodes, -1));

            cout<<"Enter" << edges << " number of edges like ( node1 , node2 , weight :)" << endl;

            for(int i=0 ; i< edges ; i++){
                
                int a , b , weight;
                cin>>a>>b>> weight ;

                graph[a][b] = weight;
                graph[b][a] = weight;
            }
        }

        void printGraph(){

            for(int i =0 ; i<nodes; i++){
                cout<<"Edges from Nodes : " << i << endl;
                for(int j = 0 ; j < nodes; j++){

                    if(i == j) continue;
                    if(graph[i][j] == -1) continue;

                    
                    cout<< "Node " << i << " to Node " << j<< " weight " << graph[i][j] <<endl;
                }
            }
        }
};

// 2. Adjacency LIST :- 

// Now Lets implement the Adjacency List implementation 

class GraphList {

    private:
        int nodes , edges ;

        map<int, vector<pair<int, int>>> graphList;

    public:

        GraphList(){

            cin>> nodes >> edges;

            for(int i = 0 ; i< edges ; i++){
                int u , v ,weight;
                cin>>u >> v >> weight;
                graphList[u].push_back({v,weight});
                graphList[v].push_back({u,weight});

            }

        }

        void printGraph(){

            for(int i = 0 ; i< nodes; i++){
                cout<< "Edges from Node " << i << " : "<<endl;
                
                for(auto & a : graphList[i] ){
                    cout<< "From Node" << i << " to Node " << a.first << " weight :" <<a.second <<endl;
                }
            }

        }

};

// 3. Graph Implementation Using EDGE list

class GraphEdges{
    private : 
        int nodes,vertices,weights;
        
        vector<tuple<int,int,int>> graph;

    public:

        GraphEdges(){
            cin>>nodes>>vertices;

            for(int i= 0 ; i< vertices ; i++){
                int u,v,w;
                cin>>u>>v>>w;
                graph.push_back({u,v,w});
            }
        }

        void printGraph(){

            for(int i=0 ; i< vertices ; i++){
                auto [u,v,w] = graph[i];
                cout<<u<<" "<<v<<" "<<w<<endl;
            }
        }
};


/* Type of Graph */

// 1. Weighted vs Unweighted
/*
    weighted graph => vector<vector<pair<int,int>>> graph(nodes, vector<int> (vertices attached , weight of the edge))

    Unweighted Graph => vector<vector<int>> graph(node , vector<int> (vertice attached to it))

*/

// 2. Directed vs Undirected
/*
{
    In Directed Graph only one way movement is allowed. In syntax we store it like this

    eg:- graph[a][b] = weight or 1 ;

    For UnDirected Graph as you have guessed.

    eg:- graph[a][b] = weightA or 1;
         graph[b][a] = weightA or 1;
}
*/
// 3. Cyclic vs Acyclic
/*
    Whether the graph is cyclic or acyclic depends on runtime input (e.g., the edges provided).

    To detect cycles, use algorithms like DFS with back-edge detection or Union-Find.
    
    For undirected connected graphs, a partial check is: If number of edges (E) > number of vertices (V) - 1, cycles exist.
    
        Note: This isn't sufficient for all cases (e.g., disconnected or directed graphs).
*/

int main(){

    GraphList abjList;

    abjList.printGraph();

    Graph mat;

    mat.printGraph();

    GraphEdges edged;

    edged.printGraph();

    return 0;
}

