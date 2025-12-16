#ifndef DISPLAY_H
#define DISPLAY_H

#include "AntAlgoritms.h"
#include "../../../libs/cpp_graph/include/Graph.h"
#include "../../../libs/cpp_graph/include/Node.h"
#include <string>
#include <vector>
#include <utility>

enum class AlgorithmMode {
    SHORTEST_PATH = 1,
    HAMILTONIAN_CYCLE = 2
};

class Display {
public:
    static void print_welcome_message();
    static void print_graph(const Graph<AntEdge>& graph);
    static AlgorithmMode get_algorithm_choice();
    static std::string get_node_name(const std::string& prompt);
    static void print_algorithm_parameters(const AntColonyOptimization& aco);
    
    static void print_shortest_path_result(
        const std::pair<double, std::vector<Node*>>& result,
        const std::string& startName,
        const std::string& endName
    );
    
    static void print_hamiltonian_cycle_result(
        const std::pair<double, std::vector<AntEdge*>>& result
    );
    
    static void write_history_to_file(
        const std::vector<double>& pheromoneHistory,
        const std::vector<double>& bestPathPheromoneHistory,
        const std::vector<double>& bestPathLengthHistory,
        const std::vector<double>& currentPathLengthHistory,
        const std::string& filename
    );
    
    static void print_error(const std::string& message);
    static void print_node_not_found_error(const std::string& nodeName);
    static void print_file_error(const std::string& filename);
    
    static void print_searching_message();
    static void print_completion_message();
};

#endif 
