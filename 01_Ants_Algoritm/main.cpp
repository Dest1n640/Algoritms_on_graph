#include "Graph.h"
#include "Edge.h"
#include "AntAlgoritms.h"
#include <iostream>
#include <memory>

int main() {
    Graph<AntEdge> g;

    auto nodeA = g.addNode("A");
    auto nodeB = g.addNode("B");
    auto nodeC = g.addNode("C");
    auto nodeD = g.addNode("D");
    auto nodeG = g.addNode("G");
    auto nodeF = g.addNode("F");
    

    g.addEdgeOne("A", "B", 3);
    g.addEdgeOne("A", "F", 1);

    g.addEdgeOne("B", "A", 3);
    g.addEdgeOne("B", "C", 8);
    g.addEdgeOne("B", "G", 3);

    g.addEdgeOne("C", "B", 3);
    g.addEdgeOne("C", "D", 1);
    g.addEdgeOne("C", "G", 1);

    g.addEdgeOne("D", "C", 8);
    g.addEdgeOne("D", "F", 1);

    g.addEdgeOne("F", "A", 3);
    g.addEdgeOne("F", "D", 3);

    g.addEdgeOne("G", "A", 3);
    g.addEdgeOne("G", "B", 3);
    g.addEdgeOne("G", "C", 3);
    g.addEdgeOne("G", "F", 4);
    g.addEdgeOne("G", "D", 5);


    std::cout << "Initial graph:" << std::endl;
    std::cout << g << std::endl;

    int numAnts = 100;
    int iterations = 50;
    
    AntColonyOptimization aco(g, numAnts);

    std::cout << "Starting Ant Colony Optimization..." << std::endl;

    auto [best_weight, best_path] = aco.HamCycle(nodeA, iterations);

    if (best_weight != -1.0) {
        std::cout << "\nAlgorithm finished." << std::endl;
        std::cout << "Shortest path length found: " << best_weight << std::endl;
    for (const auto& edge : best_path) {
        std::cout << edge->getBegin()->getName() << " -> " << edge->getEnd()->getName() << " (weight: " << edge->getWeight() << ")\n";
    }
    } else {
        std::cout << "\nAlgorithm finished." << std::endl;
        std::cout << "Path not found." << std::endl;
    }

    return 0;
}
