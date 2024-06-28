#include "../include/KruskalMST.hpp"
#include <algorithm> // for std::sort
#include <numeric> // for std::iota

Tree KruskalMST::computeMST(const Graph& graph) {
    Tree mst(graph.getAdjacencyList().size());

    // Collect all edges
    std::vector<std::tuple<int, int, int>> edges;
    for (int u = 0; u < graph.getAdjacencyList().size(); ++u) {
        for (const auto& [v, weight] : graph.getAdjacencyList()[u]) {
            if (u < v) { // Ensure each edge is considered only once
                edges.emplace_back(weight, u, v);
            }
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end());

    // Disjoint set union-find to detect cycles
    std::vector<int> parent(graph.getAdjacencyList().size());
    std::iota(parent.begin(), parent.end(), 0);

    auto find = [&](int u) {
        while (u != parent[u]) u = parent[u];
        return u;
    };

    auto union_set = [&](int u, int v) {
        parent[find(u)] = find(v);
    };

    for (const auto& [weight, u, v] : edges) {
        if (find(u) != find(v)) {
            mst.addEdge(u, v, weight);
            union_set(u, v);
        }
    }

    return mst;
}
