#include "../include/Graph.hpp"
#include "../include/Node.hpp"

template class Node<int>; // Assuming 'Node' is a class template with 'int' as the template argument.
using namespace std;
Graph::Graph() : vertices(0), edges(0) {

}

void Graph::createGraph(int vertices, int edges){

    for(int i = 0; i < vertices; i++){
        Node<int> node(i);
        nodes.push_back(node);
    }

    for(int i = 0; i < edges; i++) {
        addEdge();
    }

}

void Graph::addEdge() {
    int u, v, weight;
    cin >> u >> v >> weight;
    adjacencyList[u].emplace_back(v, weight); // u -> v
}

void Graph::addEdge(int u, int v, int weight) {
    adjacencyList[u].emplace_back(v, weight); // u -> v
}

const std::vector<std::vector<std::pair<int, int>>>& Graph::getAdjacencyList() const {
    return adjacencyList;
}
