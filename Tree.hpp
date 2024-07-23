// Tree.hpp
#pragma once

#include "Graph.hpp"

class Tree : public Graph {
public:
    Tree(const std::vector<std::pair<int, int>>& edges);
};