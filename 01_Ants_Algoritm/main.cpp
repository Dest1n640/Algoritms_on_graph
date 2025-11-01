#include "../My_graph/Graph.h"
#include "../My_graph/Node.h"
#include "../My_graph/Edge.h"
#include "AntAlgoritms.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <cstdlib>

Graph<AntEdge> makeGraph(const std::string& file_name){
    Graph<AntEdge> graph;

    std::ifstream inputFile(file_name);
    if (!inputFile.is_open()){
        std::cerr << "Could not open file " << file_name << std::endl;
        return graph;
    }

    std::string line;
    std::getline(inputFile, line); // Пропускаем заголовок

    while(std::getline(inputFile, line)){
        std::istringstream iss(line);
        std::string startName, endName;
        int weight;

        if (iss >> startName >> endName >> weight){
            // Добавляем ребро AntEdge с начальным феромоном 1.0
            graph.addEdge(startName, endName, weight, 1.0);
        }
    }
    inputFile.close();
    return graph;
}

// Печать результата для кратчайшего пути
void printShortestPath(const std::pair<double, std::vector<Node*>>& result, const std::string& startName, const std::string& endName) {
    std::cout << "\n--- Search Results ---\n";
    if (result.first < 0 || result.second.empty()) {
        std::cout << "Path from '" << startName << "' to '" << endName << "' not found.\n";
    } else {
        std::cout << "Shortest path found!\n";
        std::cout << "Total path weight (length): " << result.first << "\n";
        std::cout << "Path: ";
        for (size_t i = 0; i < result.second.size(); ++i) {
            std::cout << result.second[i]->getName();
            if (i < result.second.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "------------------------\n";
}

// Печать результата для гамильтонова цикла
void printHamiltonianCycle(const std::pair<double, std::vector<AntEdge*>>& result) {
    std::cout << "\n--- Search Results ---\n";
    if (result.first < 0 || result.second.empty()) {
        std::cout << "Hamiltonian cycle not found.\n";
    } else {
        std::cout << "Hamiltonian cycle found!\n";
        std::cout << "Total cycle weight (length): " << result.first << "\n";
        std::cout << "Path: ";
        for (size_t i = 0; i < result.second.size(); ++i) {
            std::cout << result.second[i]->getBegin()->getName();
            std::cout << " -> ";
        }
        // Замыкаем цикл
        if (!result.second.empty()) {
            std::cout << result.second.front()->getBegin()->getName();
        }
        std::cout << "\n";
    }
    std::cout << "------------------------\n";
}

void writeHistoryToFile(const std::vector<double>& history, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    outFile << "# Iteration  Total_Pheromone\n";
    for (size_t i = 0; i < history.size(); ++i) {
        outFile << i + 1 << " " << history[i] << "\n";
    }

    outFile.close();
    std::cout << "\nPheromone history has been written to " << filename << std::endl;
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    Graph<AntEdge> myGraph = makeGraph(filename);

    if (myGraph.getGraph().empty()) {
        std::cerr << "Graph is empty or could not be created." << std::endl;
        return 1;
    }

    std::cout << myGraph;
    
    int choice;
    std::cout << "\nChoose algorithm mode:\n";
    std::cout << "1. Find shortest path between two nodes\n";
    std::cout << "2. Find Hamiltonian cycle (Traveling Salesperson Problem)\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    // Используем правильный конструктор
    AntColonyOptimization aco_solver(myGraph, 100, 100);

    if (choice == 1) {
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
        // Вызываем findShortestPath и передаем результат в новую функцию печати
        auto path = aco_solver.findShortestPath(startNode, endNode);
        printShortestPath(path, startName, endName);

    } else if (choice == 2) {
        if (myGraph.getGraph().empty()) {
            std::cerr << "Graph is empty, cannot find Hamiltonian cycle." << std::endl;
            return 1;
        }
        // Берём первую попавшуюся ноду как стартовую
        Node* startNode = myGraph.getGraph().begin()->first;
        std::cout << "Starting search for Hamiltonian cycle from node '" << startNode->getName() << "'...\n";
        
        // Вызываем findHamiltonianCycle
        auto cycle = aco_solver.findHamiltonianCycle(startNode);
        printHamiltonianCycle(cycle);

    } else {
        std::cerr << "Invalid choice. Please run the program again and select 1 or 2." << std::endl;
        return 1;
    }

    writeHistoryToFile(aco_solver.getPheromoneHistory(), "output.txt");

    return 0;
}

//ИСПРАВИТЬ ЗАПИСЬ В ФАЙЛ
//Прописать py файл
