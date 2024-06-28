#ifndef KRUSKALMST_HPP
#define KRUSKALMST_HPP

#include "MSTAlgorithm.hpp"

class KruskalMST : public MSTAlgorithm {
public:
    KruskalMST() = default;
    Tree computeMST(const Graph& graph) override;
};

#endif // KRUSKALMST_HPP
