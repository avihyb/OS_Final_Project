#ifndef MSTALGORITHMFACTORY_HPP
#define MSTALGORITHMFACTORY_HPP

#include "MSTAlgorithm.hpp"

class MSTAlgorithmFactory {
public:
    enum class AlgorithmType {
        Kruskal,
        Prim
    };

    static MSTAlgorithm* createAlgorithm(AlgorithmType type);
};

#endif
