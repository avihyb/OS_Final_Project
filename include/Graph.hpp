#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility> // for std::pair
#include <iostream>
#include "Node.hpp"
#include "SFML/Graphics.hpp"

class Graph {
public:
    Graph();
    void createGraph(int vertices, int edges);
    virtual ~Graph() = default;

    void addEdge();
    void addEdge(int u, int v, int weight);
    const std::vector<std::vector<std::pair<int, int>>>& getAdjacencyList() const;

    void drawGraph(sf::RenderWindow& window);
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

protected:
    int edges;
    int vertices;
    std::vector<std::vector<std::pair<int, int>>> adjacencyList;

private:
    std::vector<Node<int>> nodes;

};

inline std::ostream& operator<<(std::ostream& os, Graph& graph) {
    os << "Launching GUI...\n";
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Graph Structure");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window
        window.clear(sf::Color::White);
        graph.drawGraph(window);
        
        window.display();
    }
    return os;
}

inline void drawGraph(sf::RenderWindow& window) {
    
}


#endif
