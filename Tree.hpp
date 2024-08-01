#pragma once

#include "Graph.hpp"
#include <vector>
#include <utility>
#include <iostream>

class Tree : public Graph {
private:
    int totalWeight;

public:

    Tree() : totalWeight(0) {}
    int calculateTotalWeight() const;
    int shortestDistance(int from, int to) const;
    std::vector<std::pair<int, int>> getEdges() const;
    // Copy assignment operator
    Tree& operator=(const Tree& other);
    void printTree() const;
};

