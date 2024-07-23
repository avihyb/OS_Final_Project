// MSTAlgorithmFactory.hpp
#ifndef MST_ALGORITHM_FACTORY_HPP
#define MST_ALGORITHM_FACTORY_HPP

#include "Graph.hpp"
#include "Tree.hpp"
#include <memory>
#include <string>

class MSTAlgorithm {
public:
    virtual Tree findMST(const Graph& graph) = 0;
    virtual ~MSTAlgorithm() = default;
};

class KruskalMST : public MSTAlgorithm {
public:
    Tree findMST(const Graph& graph) override;
};

class PrimMST : public MSTAlgorithm {
public:
    Tree findMST(const Graph& graph) override;
};

class MSTAlgorithmFactory {
public:
    static std::unique_ptr<MSTAlgorithm> createAlgorithm(const std::string& type);
};

#endif // MST_ALGORITHM_FACTORY_HPP