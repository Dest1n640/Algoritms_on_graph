#include "../include/Display.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void Display::print_welcome_message() {
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║     ANT COLONY OPTIMIZATION ALGORITHM         ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    std::cout << "\nThis program uses ant colony optimization to solve:\n";
    std::cout << "  1. Shortest Path Problem\n";
    std::cout << "  2. Traveling Salesperson Problem (Hamiltonian Cycle)\n";
    std::cout << std::endl;
}

void Display::print_graph(const Graph<AntEdge>& graph) {
    std::cout << "\n--- Graph Information ---\n";
    std::cout << graph;
    std::cout << "------------------------\n";
}

AlgorithmMode Display::get_algorithm_choice() {
    int choice;
    std::cout << "\nChoose algorithm mode:\n";
    std::cout << "  1. Find shortest path between two nodes\n";
    std::cout << "  2. Find Hamiltonian cycle (Traveling Salesperson Problem)\n";
    std::cout << "\nEnter your choice (1 or 2): ";
    std::cin >> choice;
    std::cin.ignore(); // Очищаем буфер ввода
    
    if (choice == 1) {
        return AlgorithmMode::SHORTEST_PATH;
    } else if (choice == 2) {
        return AlgorithmMode::HAMILTONIAN_CYCLE;
    } else {
        std::cout << "Invalid choice. Defaulting to shortest path.\n";
        return AlgorithmMode::SHORTEST_PATH;
    }
}

std::string Display::get_node_name(const std::string& prompt) {
    std::string nodeName;
    std::cout << prompt;
    std::cin >> nodeName;
    return nodeName;
}

void Display::print_algorithm_parameters(const AntColonyOptimization& aco) {
    std::cout << "\n┌─────────────────────────────────────────┐\n";
    std::cout << "│     Algorithm Parameters                │\n";
    std::cout << "├─────────────────────────────────────────┤\n";
    std::cout << "│ Evaporation Rate:  " << std::setw(10) << std::fixed 
              << std::setprecision(2) << aco.getEvaporationRate() << "        │\n";
    std::cout << "│ Iterations:        " << std::setw(10) << aco.getIterations() 
              << "        │\n";
    std::cout << "│ Stagnation Limit:  " << std::setw(10) << aco.getStagnationLimit() 
              << "        │\n";
    std::cout << "└─────────────────────────────────────────┘\n";
}

void Display::print_shortest_path_result(
    const std::pair<double, std::vector<Node*>>& result,
    const std::string& startName,
    const std::string& endName) {
    
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║            SHORTEST PATH RESULTS               ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    
    if (result.first < 0 || result.second.empty()) {
        std::cout << "\n⚠️  Path from '" << startName << "' to '" << endName 
                  << "' not found.\n";
    } else {
        std::cout << "\n✓ Shortest path found!\n";
        std::cout << "  Total path weight (length): " << std::fixed 
                  << std::setprecision(2) << result.first << "\n\n";
        std::cout << "  Path: ";
        
        for (size_t i = 0; i < result.second.size(); ++i) {
            std::cout << result.second[i]->getName();
            if (i < result.second.size() - 1) {
                std::cout << " → ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n════════════════════════════════════════════════\n";
}

void Display::print_hamiltonian_cycle_result(
    const std::pair<double, std::vector<AntEdge*>>& result) {
    
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║        HAMILTONIAN CYCLE RESULTS               ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    
    if (result.first < 0 || result.second.empty()) {
        std::cout << "\n⚠️  Hamiltonian cycle not found.\n";
    } else {
        std::cout << "\n✓ Hamiltonian cycle found!\n";
        std::cout << "  Total cycle weight (length): " << std::fixed 
                  << std::setprecision(2) << result.first << "\n\n";
        std::cout << "  Cycle: ";
        
        for (size_t i = 0; i < result.second.size(); ++i) {
            std::cout << result.second[i]->getBegin()->getName();
            std::cout << " → ";
        }
        
        // Замыкаем цикл
        if (!result.second.empty()) {
            std::cout << result.second.front()->getBegin()->getName();
        }
        std::cout << "\n";
    }
    std::cout << "\n════════════════════════════════════════════════\n";
}

void Display::write_history_to_file(
    const std::vector<double>& pheromoneHistory,
    const std::vector<double>& bestPathPheromoneHistory,
    const std::vector<double>& bestPathLengthHistory,
    const std::vector<double>& currentPathLengthHistory,
    const std::string& filename) {
    
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        print_file_error(filename);
        return;
    }

    outFile << "# Iteration  Total_Pheromone  Best_Path_Pheromone  "
            << "Best_Path_Length  Current_Path_Length\n";
    
    for (size_t i = 0; i < pheromoneHistory.size(); ++i) {
        outFile << (i + 1) << " " << pheromoneHistory[i];
        
        if (i < bestPathPheromoneHistory.size()) {
            outFile << " " << bestPathPheromoneHistory[i];
        }
        if (i < bestPathLengthHistory.size()) {
            outFile << " " << bestPathLengthHistory[i];
        }
        if (i < currentPathLengthHistory.size()) {
            outFile << " " << currentPathLengthHistory[i];
        }
        outFile << "\n";
    }

    outFile.close();
    std::cout << "\n✓ Pheromone history has been written to: " << filename << "\n";
}

void Display::print_error(const std::string& message) {
    std::cerr << "\n❌ Error: " << message << "\n";
}

void Display::print_node_not_found_error(const std::string& nodeName) {
    std::cerr << "\n❌ Error: Node '" << nodeName << "' not found in the graph.\n";
}

void Display::print_file_error(const std::string& filename) {
    std::cerr << "\n❌ Error: Could not open file '" << filename << "'\n";
}

void Display::print_searching_message() {
    std::cout << "\n🔍 Searching for optimal solution...\n";
}

void Display::print_completion_message() {
    std::cout << "\n✓ Algorithm execution completed.\n";
}
