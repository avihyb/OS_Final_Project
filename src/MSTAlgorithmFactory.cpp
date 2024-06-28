#include "../include/MSTAlgorithmFactory.hpp"
#include "../include/KruskalMST.hpp"
#include "../include/MSTAlgorithm.hpp"

MSTAlgorithm* MSTAlgorithmFactory::createAlgorithm(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::Kruskal:
            return new KruskalMST();
        
        case AlgorithmType::Prim:
            // return new PrimMST();
            return nullptr;
       
        default:
            return nullptr;
    }
}
