#include "../include/AntAlgoritms.h"
#include "../include/Display.h"
#include "../../../libs/cpp_graph/include/Edge.h"
#include "../../../libs/cpp_graph/include/Graph.h"
#include "../../../libs/cpp_graph/include/Node.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

Graph<AntEdge> makeGraph(const std::string& file_name) {
    Graph<AntEdge> graph;

    std::ifstream inputFile(file_name);
    if (!inputFile.is_open()) {
        Display::print_file_error(file_name);
        return graph;
    }

    std::string line;
    std::getline(inputFile, line);
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string startName, endName;
        int weight;

        if (iss >> startName >> endName >> weight) {
            graph.addEdge(startName, endName, weight, 1.0);
        }
    }
    
    inputFile.close();
    return graph;
}

void handle_shortest_path_mode(AntColonyOptimization& aco, Graph<AntEdge>& graph) {
    std::string startName = Display::get_node_name("Enter the start node: ");
    std::string endName = Display::get_node_name("Enter the end node: ");

    Node* startNode = graph.findNode(startName);
    Node* endNode = graph.findNode(endName);

    if (!startNode) {
        Display::print_node_not_found_error(startName);
        return;
    }
    if (!endNode) {
        Display::print_node_not_found_error(endName);
        return;
    }

    Display::print_searching_message();
    auto result = aco.findShortestPath(startNode, endNode);
    Display::print_shortest_path_result(result, startName, endName);
}

void handle_hamiltonian_cycle_mode(AntColonyOptimization& aco, Graph<AntEdge>& graph) {
    if (graph.getGraph().empty()) {
        Display::print_error("Graph is empty, cannot find Hamiltonian cycle.");
        return;
    }

    Node* startNode = graph.getGraph().begin()->first;
    std::cout << "\nStarting search from node: '" << startNode->getName() << "'\n";
    
    Display::print_searching_message();
    auto result = aco.findHamiltonianCycle(startNode);
    Display::print_hamiltonian_cycle_result(result);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        Display::print_error("Usage: " + std::string(argv[0]) + " <graph_file>");
        return 1;
    }

    Display::print_welcome_message();

    const std::string filename = argv[1];
    Graph<AntEdge> graph = makeGraph(filename);

    if (graph.getGraph().empty()) {
        Display::print_error("Graph is empty or could not be created from file: " + filename);
        return 1;
    }

    Display::print_graph(graph);

    try {
        AntColonyOptimization aco(graph, 
                                  "config/ant.cfg",
                                  "config/alg_param.cfg");
        
        Display::print_algorithm_parameters(aco);

        AlgorithmMode mode = Display::get_algorithm_choice();

        if (mode == AlgorithmMode::SHORTEST_PATH) {
            handle_shortest_path_mode(aco, graph);
        } else {
            handle_hamiltonian_cycle_mode(aco, graph);
        }

        Display::write_history_to_file(
            aco.getPheromoneHistory(),
            aco.getBestPathPheromoneHistory(),
            aco.getBestPathLengthHistory(),
            aco.getCurrentPathLengthHistory(),
            "data/output.txt"
        );

        Display::print_completion_message();

    } catch (const AntAlgorithmException& e) {
        Display::print_error(std::string("Algorithm error: ") + e.what());
        return 1;
    } catch (const std::exception& e) {
        Display::print_error(std::string("Unexpected error: ") + e.what());
        return 1;
    }

    return 0;
}
