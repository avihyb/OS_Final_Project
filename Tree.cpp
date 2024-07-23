// Tree.cpp
#include "Tree.hpp"

Tree::Tree(const std::vector<std::pair<int, int>>& edges) {
    for (const auto& edge : edges) {
        addEdge(edge.first, edge.second, 1);  // Weight is set to 1 for simplicity
    }
}