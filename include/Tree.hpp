#ifndef TREE_HPP
#define TREE_HPP

#include "Graph.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

class Tree : public Graph {
public:
    Tree(int vertices);
    const std::vector<std::vector<std::pair<int, int>>>& getAdjacencyList() const;
    void longestDistance(int source, int destination);
    void avgDistanceBetweenTwoEdges();
    void shortestDistanceBetweenTwoVertices(int source, int destination);
    friend std::ostream& operator<<(std::ostream& os, Tree& tree);
    static void drawTree(sf::RenderWindow& window);
};


inline std::ostream& operator<<(std::ostream& os, Tree& tree) {
    sf::RenderWindow window;
    os << "Launching GUI...\n";
    window.create(sf::VideoMode(800, 600), "Tree Structure");
     while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window
        window.clear(sf::Color::White);
        tree.drawTree(window);
        
        window.display();
    }
    return os;
}

inline void drawTree(sf::RenderWindow& window) {
  
}
#endif
