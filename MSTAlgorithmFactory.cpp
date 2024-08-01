#include "MSTAlgorithmFactory.hpp"
#include "Graph.hpp"
#include "Tree.hpp"
#include <algorithm>
#include <vector>
#include <queue>
#include <functional>  // For std::greater<>
#include <limits>  // For std::numeric_limits

// Class for Disjoint Set (used in Kruskal's algorithm)
class DisjointSet {
private:
    std::vector<int> parent, rank;

public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int xroot = find(x), yroot = find(y);
        if (rank[xroot] < rank[yroot])
            parent[xroot] = yroot;
        else if (rank[xroot] > rank[yroot])
            parent[yroot] = xroot;
        else {
            parent[yroot] = xroot;
            rank[xroot]++;
        }
    }
};

// Implementation of Kruskal's algorithm
Tree KruskalMST::findMST(const Graph& graph) {
    std::vector<std::pair<int, std::pair<int, int>>> edges = graph.getAllEdges();
    std::sort(edges.begin(), edges.end());

    int V = graph.getVertexCount();
    DisjointSet ds(V);
    Tree mst;

    for (const auto& edge : edges) {
        int weight = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        if (set_u != set_v) {
            mst.addEdge(u, v, weight);
            ds.unite(set_u, set_v);
        }
    }

    return mst;
}

Tree PrimMST::findMST(const Graph& graph) {
    int V = graph.getVertexCount();
    std::vector<bool> inMST(V, false);  // Track vertices included in MST
    std::vector<int> key(V, std::numeric_limits<int>::max()); // Key values for vertices
    std::vector<int> parent(V, -1);  // To store the constructed MST

    // Min-heap (priority queue) to pick the minimum weight edge at each step
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    // Start from the first vertex (0)
    pq.push({0, 0});  // {weight, vertex}
    key[0] = 0;

    Tree mst;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // If vertex is already included in MST, skip it
        if (inMST[u]) continue;

        // Include vertex in MST
        inMST[u] = true;

        // Traverse all adjacent vertices of u
        for (const auto& edge : graph.getAdjacentEdges(u)) {
            int v = edge.to;
            int weight = edge.weight;

            // If v is not in MST and weight of (u, v) is less than current key of v
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                pq.push({key[v], v});
                parent[v] = u;
            }
        }
    }

    // Construct the MST
    for (int i = 1; i < V; ++i) {
        if (parent[i] != -1) {
            mst.addEdge(parent[i], i, key[i]);
        }
    }

    return mst;
}

// Factory method implementation
std::unique_ptr<MSTAlgorithm> MSTAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "Kruskal") {
        return std::make_unique<KruskalMST>();
    } else if (type == "Prim") {
        return std::make_unique<PrimMST>();
    }
    return nullptr;
}
