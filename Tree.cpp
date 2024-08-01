// Tree.cpp
#include "Tree.hpp"
#include <climits>
#include <queue>
#include <vector>



int Tree::calculateTotalWeight() const {
    int totalWeight = 0;
    for (const auto& [from, edges] : adjacencyList) {
        for (const auto& edge : edges) {
            totalWeight += edge.weight;
        }
    }
    return totalWeight;
}

int Tree::shortestDistance(int from, int to) const {
    // Implement Dijkstra's algorithm for shortest path
    std::vector<int> dist(adjacencyList.size(), INT_MAX);
    dist[from] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, from});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (const auto& edge : adjacencyList.at(u)) {
            int v = edge.to, weight = edge.weight;
            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist[to];
}



Tree& Tree::operator=(const Tree& other) {
    if (this == &other) {
        return *this; // handle self-assignment
    }

    // Call the base class assignment operator
    Graph::operator=(other);

    // Copy all members
    this->totalWeight = other.totalWeight;

    // Copy other members of Tree if any

    return *this;
}

void Tree::printTree() const {
    for (const auto& [from, edges] : adjacencyList) {
        for (const auto& edge : edges) {
            std::cout << from << " -> " << edge.to << " : " << edge.weight << std::endl;
        }
    }
}

std::vector<std::pair<int, int>> Tree::getEdges() const {
    std::vector<std::pair<int, int>> edges;
    for (const auto& [from, adjList] : adjacencyList) {
        for (const auto& edge : adjList) {
            edges.push_back({from, edge.to});
        }
    }
    return edges;
}