#include "Graph.h"
#include "Edge.h"
#include "AntAlgoritms.h"
#include <iostream>
#include <memory>

int main() {
    // 1. Создание графа
    Graph<AntEdge> g;

    // 2. Добавление узлов
    auto nodeA = g.addNode("A");
    auto nodeB = g.addNode("B");
    auto nodeC = g.addNode("C");
    auto nodeD = g.addNode("D");

    // 3. Добавление ребер (с начальным феромоном 1.0 по умолчанию)
    g.addEdge("A", "B", 10);
    g.addEdge("A", "C", 2);
    g.addEdge("C", "B", 3);
    g.addEdge("B", "D", 5);
    g.addEdge("C", "D", 20);

    std::cout << "Initial graph:" << std::endl;
    std::cout << g << std::endl;

    // 4. Настройка и запуск муравьиного алгоритма
    int numAnts = 10;
    int iterations = 50;
    
    AntColonyOptimization aco(g, numAnts);

    std::cout << "Starting Ant Colony Optimization..." << std::endl;
    std::cout << "Finding path from " << nodeA->getName() << " to " << nodeD->getName() << std::endl;

    double bestPathLength = aco.run(nodeA, nodeD, iterations);

    // 5. Вывод результата
    if (bestPathLength != -1.0) {
        std::cout << "\nAlgorithm finished." << std::endl;
        std::cout << "Shortest path length found: " << bestPathLength << std::endl;
    } else {
        std::cout << "\nAlgorithm finished." << std::endl;
        std::cout << "Path not found." << std::endl;
    }

    return 0;
}
