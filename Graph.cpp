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

    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(center.x + radius * std::cos(fromAngle), center.y + radius * std::sin(fromAngle))),
        sf::Vertex(sf::Vector2f(center.x + radius * std::cos(toAngle), center.y + radius * std::sin(toAngle)))
    };

    line[0].color = isMST ? sf::Color::Red : sf::Color::Black;
    line[1].color = isMST ? sf::Color::Red : sf::Color::Black;

    window.draw(line, 2, sf::Lines);

    // Draw weight
    sf::Text text;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setString(std::to_string(weight));
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Blue);
    text.setPosition((line[0].position + line[1].position) / 2.0f);

    window.draw(text);
}

std::vector<std::pair<int, int>> Graph::findMST() const {
    // Implement Kruskal's algorithm for MST
    std::vector<std::pair<int, std::pair<int, int>>> edges;
    for (const auto& [from, adjList] : adjacencyList) {
        for (const auto& edge : adjList) {
            edges.push_back({edge.weight, {from, edge.to}});
        }
    }
    std::sort(edges.begin(), edges.end());

    std::vector<int> parent(adjacencyList.size());
    for (int i = 0; i < parent.size(); ++i) parent[i] = i;

    std::function<int(int)> find = [&](int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };

    std::vector<std::pair<int, int>> mst;
    for (const auto& edge : edges) {
        int u = edge.second.first, v = edge.second.second;
        if (find(u) != find(v)) {
            mst.push_back({u, v});
            parent[find(u)] = find(v);
        }
    }

    return mst;
}