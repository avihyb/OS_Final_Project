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
    int getVertexCount() const;
    std::vector<std::pair<int, std::pair<int, int>>> getAllEdges() const;

    // Returns a vector of edges adjacent to a given vertex
    std::vector<Edge> getAdjacentEdges(int vertex) const {
        auto it = adjacencyList.find(vertex);
        if (it != adjacencyList.end()) {
            return it->second;
        }
        return {}; // Return an empty vector if the vertex is not found
    }

    bool isEmpty() { return adjacencyList.empty(); }
    
protected:
    virtual void drawVertex(sf::RenderWindow& window, int vertex, float x, float y) const;
    virtual void drawEdge(sf::RenderWindow& window, int from, int to, int weight, bool isMST) const;
};
