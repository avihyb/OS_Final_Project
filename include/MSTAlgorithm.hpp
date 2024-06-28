#ifndef MSTALGORITHM_HPP
#define MSTALGORITHM_HPP

#include "Tree.hpp"
#include "Graph.hpp"

class MSTAlgorithm {
public:
    virtual ~MSTAlgorithm() = default;
    virtual Tree computeMST(const Graph& graph) = 0;
};

#endif
