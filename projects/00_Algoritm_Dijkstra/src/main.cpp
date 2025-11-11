#include "../include/Dijkstra.h"
#include "../../../libs/cpp_graph/include/Edge.h"
#include "../../../libs/cpp_graph/include/Graph.h"
#include "../../../libs/cpp_graph/include/Node.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

Graph<Edge> makeGraph(const std::string& file_name){
    Graph<Edge> graph;

    std::ifstream inputFile(file_name);
    if (!inputFile.is_open()){
        std::cerr << "Could not open file " << file_name << std::endl;
        return graph;
    }

    std::string line;
    std::getline(inputFile, line);

    while(std::getline(inputFile, line)){
        std::istringstream iss(line);
        std::string startName, endName;
        int weight;

        if (iss >> startName >> endName >> weight){
            graph.addEdge(startName, endName, weight);
        }
    }
    inputFile.close();
    return graph;
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    Graph<Edge> myGraph = makeGraph(filename);

    if (myGraph.getGraph().empty()) {
        std::cerr << "Graph is empty or could not be created." << std::endl;
        return 1;
    }

    std::cout << myGraph;
    std::string startName, endName;

    std::cout << "Enter the start node: ";
    std::cin >> startName;
    std::cout << "Enter the end node: ";
    std::cin >> endName;

    Node* startNode = myGraph.findNode(startName);
    Node* endNode = myGraph.findNode(endName);

    if (startNode == nullptr || endNode == nullptr) {
        std::cerr << "One or both nodes ('" << startName << "', '" << endName << "') were not found in the graph." << std::endl;
        return 1;
    }

    Dijkstra dijkstra_solver(myGraph);
    Way path = dijkstra_solver.shortestWay(startNode, endNode);

    std::cout << "\n--- Search Results ---\n";
    if (path.length < 0) {
        std::cout << "Path from '" << startName << "' to '" << endName << "' not found.\n";
    } else {
        std::cout << "Shortest path found!\n";
        std::cout << "Total path weight (length): " << path.length << "\n";
        std::cout << "Path: ";
        for (size_t i = 0; i < path.nodes.size(); ++i) {
            std::cout << path.nodes[i]->getName();
            if (i < path.nodes.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "------------------------\n";

    return 0;
}
