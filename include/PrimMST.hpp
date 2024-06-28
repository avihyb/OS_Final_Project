#ifndef PRIM_HPP
#define PRIM_HPP

#include "MSTAlgorithm.hpp"

class PrimMST : public MSTAlgorithm {
public:
    PrimMST() = default;
    Tree computeMST(const Graph& graph) override;
};

#endif 
