// Graph.hpp
#pragma once

#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

class Graph {
protected:
    struct Edge {
        int to;
        int weight;
    };

    std::unordered_map<int, std::vector<Edge>> adjacencyList;

public:
    
    virtual void addEdge(int from, int to, int weight);
    virtual void drawGraph(sf::RenderWindow& window, const std::vector<std::pair<int, int>>& mstEdges = {}) const;
    std::vector<std::pair<int, int>> findMST() const;

protected:
    virtual void drawVertex(sf::RenderWindow& window, int vertex, float x, float y) const;
    virtual void drawEdge(sf::RenderWindow& window, int from, int to, int weight, bool isMST) const;
};