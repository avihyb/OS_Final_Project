// Graph.cpp
#include "Graph.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>


using namespace std;

void Graph::addEdge(int from, int to, int weight) {
    adjacencyList[from].push_back({to, weight});
}

void Graph::drawGraph(sf::RenderWindow& window, const std::vector<std::pair<int, int>>& mstEdges) const {
    const float radius = 200.0f;
    const sf::Vector2f center(400, 300);
    const int vertexCount = adjacencyList.size();

    for (const auto& [from, edges] : adjacencyList) {
        float angle = 2 * M_PI * from / vertexCount;
        float x = center.x + radius * std::cos(angle);
        float y = center.y + radius * std::sin(angle);

        drawVertex(window, from, x, y);

        for (const auto& edge : edges) {
            float toAngle = 2 * M_PI * edge.to / vertexCount;
            float toX = center.x + radius * std::cos(toAngle);
            float toY = center.y + radius * std::sin(toAngle);

            bool isMST = std::find(mstEdges.begin(), mstEdges.end(), std::make_pair(from, edge.to)) != mstEdges.end();
            drawEdge(window, from, edge.to, edge.weight, isMST);
        }
    }
}

void Graph::drawVertex(sf::RenderWindow& window, int vertex, float x, float y) const {
    sf::CircleShape shape(20);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
    shape.setPosition(x - 20, y - 20);

    sf::Text text;
    sf::Font font;
    font.loadFromFile("arial.ttf");  // Make sure to have this font file in your project directory
    text.setFont(font);
    text.setString(std::to_string(vertex));
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x - 10, y - 15);

    window.draw(shape);
    window.draw(text);
}

void Graph::drawEdge(sf::RenderWindow& window, int from, int to, int weight, bool isMST) const {
    const float radius = 200.0f;
    const sf::Vector2f center(400, 300);
    const int vertexCount = adjacencyList.size();

    float fromAngle = 2 * M_PI * from / vertexCount;
    float toAngle = 2 * M_PI * to / vertexCount;

    sf::Vector2f fromPos(center.x + radius * std::cos(fromAngle), center.y + radius * std::sin(fromAngle));
    sf::Vector2f toPos(center.x + radius * std::cos(toAngle), center.y + radius * std::sin(toAngle));

    sf::Color lineColor = isMST ? sf::Color::Red : sf::Color::Black;
    int thickness = isMST ? 3 : 1;  // Adjust the thickness as needed

    for (int i = -thickness / 2; i <= thickness / 2; ++i) {
        for (int j = -thickness / 2; j <= thickness / 2; ++j) {
            sf::Vertex line[] = {
                sf::Vertex(fromPos + sf::Vector2f(i, j)),
                sf::Vertex(toPos + sf::Vector2f(i, j))
            };

            line[0].color = lineColor;
            line[1].color = lineColor;

            window.draw(line, 2, sf::Lines);
        }
    }

    // Draw weight
    sf::Text text;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setString(std::to_string(weight));
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Blue);
    text.setPosition((fromPos + toPos) / 2.0f);

    window.draw(text);
}


int Graph::getVertexCount() const {
    return adjacencyList.size();
}

std::vector<std::pair<int, std::pair<int, int>>> Graph::getAllEdges() const {
    std::vector<std::pair<int, std::pair<int, int>>> edges;
    for (const auto& [from, adjList] : adjacencyList) {
        for (const auto& edge : adjList) {
            edges.push_back({edge.weight, {from, edge.to}});
        }
    }
    return edges;
}

