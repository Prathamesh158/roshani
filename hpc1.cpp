// #include <iostream>
// #include <vector>
// #include <queue>
// #include <omp.h>
 #include <bits/stdc++.h>
using namespace std;

struct Edge {
    int src, dest;
};

class Graph {
    int V; // Number of vertices

public:
    vector<vector<int>> adj; // Adjacency list

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int src, int dest) {
        adj[src].push_back(dest);
        adj[dest].push_back(src); // Since it's an undirected graph
    }

void parallelBFS(int start) {
    vector<bool> visited(V, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        cout << u << " ";
        q.pop();

        #pragma omp parallel for
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (!visited[v]) {
                #pragma omp critical
                {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
}


    // Depth First Search
    void parallelDFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i) {
            int u = adj[v][i];
            if (!visited[u]) {
                parallelDFSUtil(u, visited);
            }
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        parallelDFSUtil(start, visited);
    }
};

int main() {
    int V, E; // V = Number of vertices, E = Number of edges
    cout << "Enter the number of vertices and edges: ";
    cin >> V >> E;

    Graph graph(V);

    cout << "Enter " << E << " edges (source destination):" << endl;
    for (int i = 0; i < E; ++i) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    int startVertex;
    cout << "Enter the starting vertex for traversal: ";
    cin >> startVertex;

    // Perform BFS
    cout << "BFS traversal starting from vertex " << startVertex << ": ";
    graph.parallelBFS(startVertex);
    cout << endl;

    // Perform DFS
    cout << "DFS traversal starting from vertex " << startVertex << ": ";
    graph.parallelDFS(startVertex);
    cout << endl;

    return 0;
}

/*
Enter the number of vertices and edges: 5 4 
Enter 4 edges (source destination):
0 1
0 2
1 3
1 4
Enter the starting vertex for traversal: 0
BFS traversal starting from vertex 0: 0 1 2 3 4 
DFS traversal starting from vertex 0: 0 1 3 4 2 
*/