// MSTAlgorithmFactory.cpp
#include "MSTAlgorithmFactory.hpp"
#include "Tree.hpp"
#include <algorithm>
#include <vector>

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
    Tree mst(V);

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

// Implementation of Prim's algorithm
Tree PrimMST::findMST(const Graph& graph) {
    // This is just a placeholder implementation
    // You should implement Prim's algorithm here
    return Tree(graph.getVertexCount());
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