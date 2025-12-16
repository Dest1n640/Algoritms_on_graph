#include "../include/AntAlgoritms.h"
#include "../../../libs/cpp_graph/include/Edge.h"
#include "../../../libs/cpp_graph/include/Graph.h"
#include "../../../libs/cpp_graph/include/Node.h"
#include <random>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>


AntAlgorithmException::AntAlgorithmException(const std::string& message)
    : std::runtime_error(message) {}

void AntColonyOptimization::resetAnts(Node* start){
  for(auto& ant : ants) 
    ant.reset(start);
}

void AntColonyOptimization::runIteration(Node* start, Node* end){
  resetAnts(start);
  for (auto& ant : ants){
    while(ant.getCurrentNode() != end){
      Node* currnode = ant.getCurrentNode();
      std::vector<AntEdge>& all_neighbors = graph.getGraphNonConst().at(currnode);
      
      std::vector<AntEdge*> available_neighbors;
      for(AntEdge& edge : all_neighbors){
        if (!ant.is_visited(edge.getEnd())){
          available_neighbors.push_back(&edge);
        }
      }

      if (available_neighbors.empty()){
        break; 
      }

      AntEdge* chosen_edge = ant.chooseNextNode(available_neighbors);
      
      if (chosen_edge){
        ant.visitNode(chosen_edge);
      } else {
        break;
      }
    }
  }
}

void AntColonyOptimization::updatePheromones(Node* endNode) {
    // 1. испарение
    for (auto& pair : graph.getGraphNonConst()) {
        for (auto& edge : pair.second) {
            double currentpheromone = edge.getPheromone();
            edge.setPheromone(currentpheromone * (1.0 - evaporationRate));
        }
    }

    // 2. Отложение
    for (const auto& ant : ants) {
        const auto& path = ant.getPath();
        double pathLength = ant.getPathLength();
        if (pathLength > 0 && ant.getCurrentNode() == endNode) {
            double deltaPheromone = ant.getPheromoneDeposit() / ant.getPathLength();
            for (AntEdge* edge : path) {
                double currentPheromone = edge->getPheromone();
                edge->setPheromone(currentPheromone + deltaPheromone);
            }
        }
    }
}

AntColonyOptimization::AntColonyOptimization(Graph<AntEdge>& g, 
                                             const std::string& antConfigFile,
                                             const std::string& algConfigFile)
    : graph(g), evaporationRate(0.0), iterations(0), stagnation_limit(0) {
    
    std::cout << "Initializing Ant Colony Optimization..." << std::endl;
    
    loadAlgorithmParameters(algConfigFile);
    loadAntsFromConfig(antConfigFile);
}


std::pair<double, std::vector<Node*>> AntColonyOptimization::findShortestPath(Node* startNode, Node* endNode) {
    double bestPathLength = -1.0;
    std::vector<Node*> bestPath;
    pheromoneHistory.clear();
    bestPathPheromoneHistory.clear();
    bestPathLengthHistory.clear();
    currentPathLengthHistory.clear();
    int no_change_counter = 0;
    double previousBestLength = -1.0;

    for (int i = 0; i < iterations; ++i) {
        runIteration(startNode, endNode);
        updatePheromones(endNode);
        recordPheromoneState();

        double currentIterationBest = -1.0;
        for (const auto& ant : ants) {
            if (ant.getCurrentNode() == endNode) {
                double currentPathLength = ant.getPathLength();
                if (currentIterationBest < 0 || currentPathLength < currentIterationBest) {
                    currentIterationBest = currentPathLength;
                }
                if (bestPathLength < 0 || currentPathLength < bestPathLength) {
                    bestPathLength = currentPathLength;
                    bestPath.clear();
                    bestPath.push_back(startNode);
                    for(AntEdge* edge : ant.getPath()){
                        bestPath.push_back(edge->getEnd());
                    }
                }
            }
        }
        
        // Проверка на застой: если лучший путь не улучшился
        if (i > 0 && previousBestLength > 0) {
            if (std::abs(bestPathLength - previousBestLength) < 1e-9) {
                no_change_counter++;
            } else {
                no_change_counter = 0;
            }
        }
        previousBestLength = bestPathLength;

        if (no_change_counter >= stagnation_limit) {
            std::cout << "\nAlgorithm stopped: best path hasn't improved for " << stagnation_limit << " iterations (stopped at iteration " << i + 1 << ")." << std::endl;
            break;
        }
        
        // Записываем глобально лучший результат и текущий результат итерации
        recordBestPathLength(bestPathLength);
        recordCurrentPathLength(currentIterationBest > 0 ? currentIterationBest : bestPathLength);
        recordBestPathPheromone(bestPath);
    }
    return {bestPathLength, bestPath};
}

void AntColonyOptimization::recordPheromoneState() {
    double totalPheromone = 0.0;
    for (const auto& pair : graph.getGraph()) {
        for (const AntEdge& edge : pair.second) {
            totalPheromone += edge.getPheromone();
        }
    }
    pheromoneHistory.push_back(totalPheromone / 2.0);
}

// Измененный метод для получения истории
const std::vector<double>& AntColonyOptimization::getPheromoneHistory() const {
    return pheromoneHistory;
}

void AntColonyOptimization::recordBestPathLength(double bestLength) {
    bestPathLengthHistory.push_back(bestLength);
}

const std::vector<double>& AntColonyOptimization::getBestPathLengthHistory() const {
    return bestPathLengthHistory;
}

void AntColonyOptimization::recordCurrentPathLength(double currentLength) {
    currentPathLengthHistory.push_back(currentLength);
}

const std::vector<double>& AntColonyOptimization::getCurrentPathLengthHistory() const {
    return currentPathLengthHistory;
}

double AntColonyOptimization::getEvaporationRate() const{
  return evaporationRate;
}
int AntColonyOptimization::getIterations() const{
  return iterations;
}
int AntColonyOptimization::getStagnationLimit() const{
  return stagnation_limit;
}

// Для записи феромонов на лучшем пути (версия для Node*)
void AntColonyOptimization::recordBestPathPheromone(const std::vector<Node*>& path) {
    if (path.size() < 2) {
        bestPathPheromoneHistory.push_back(0.0);
        return;
    }
    
    double totalPheromone = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        AntEdge* edge = graph.findEdge(path[i], path[i + 1]);
        if (edge) {
            totalPheromone += edge->getPheromone();
        }
    }
    bestPathPheromoneHistory.push_back(totalPheromone);
}

// Для записи феромонов на лучшем пути (версия для AntEdge*)
void AntColonyOptimization::recordBestPathPheromone(const std::vector<AntEdge*>& path) {
    double totalPheromone = 0.0;
    for (AntEdge* edge : path) {
        if (edge) {
            totalPheromone += edge->getPheromone();
        }
    }
    bestPathPheromoneHistory.push_back(totalPheromone);
}

void AntColonyOptimization::validateAlgorithmParameters() const {
    if (evaporationRate < 0.0 || evaporationRate > 1.0)
        throw AntAlgorithmException("Invalid evaporationRate");
    if (iterations <= 0) 
        throw AntAlgorithmException("Invalid iterations");
    if (stagnation_limit <= 0) 
        throw AntAlgorithmException("Invalid stagnation_limit");
}

void AntColonyOptimization::loadAlgorithmParameters(const std::string& configFile) {
    std::ifstream file(configFile);
    
    if (!file.is_open()) {
        throw AntAlgorithmException("Could not open algorithm parameters file: " + configFile);
    }
    
    bool foundEvaporation = false;
    bool foundIterations = false;
    bool foundStagnation = false;
    
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string paramName;
        
        if (iss >> paramName) {
            if (paramName == "evaporationRate") {
                if (!(iss >> evaporationRate)) {
                    throw AntAlgorithmException("Invalid value for evaporationRate at line " + 
                                               std::to_string(lineNumber) + " in " + configFile);
                }
                foundEvaporation = true;
                std::cout << "Loaded evaporationRate: " << evaporationRate << std::endl;
            }
            else if (paramName == "iterations") {
                if (!(iss >> iterations)) {
                    throw AntAlgorithmException("Invalid value for iterations at line " + 
                                               std::to_string(lineNumber) + " in " + configFile);
                }
                foundIterations = true;
                std::cout << "Loaded iterations: " << iterations << std::endl;
            }
            else if (paramName == "stagnation_limit") {
                if (!(iss >> stagnation_limit)) {
                    throw AntAlgorithmException("Invalid value for stagnation_limit at line " + 
                                               std::to_string(lineNumber) + " in " + configFile);
                }
                foundStagnation = true;
                std::cout << "Loaded stagnation_limit: " << stagnation_limit << std::endl;
            }
        }
    }
    
    file.close();
    
    if (!foundEvaporation) {
        throw AntAlgorithmException("Missing parameter 'evaporationRate' in " + configFile);
    }
    if (!foundIterations) {
        throw AntAlgorithmException("Missing parameter 'iterations' in " + configFile);
    }
    if (!foundStagnation) {
        throw AntAlgorithmException("Missing parameter 'stagnation_limit' in " + configFile);
    }
    
    validateAlgorithmParameters();
}

void AntColonyOptimization::loadAntsFromConfig(const std::string& configFile) {
    std::ifstream file(configFile);

    if (!file.is_open()) {
        throw AntAlgorithmException("Could not open ants configuration file: " + configFile);
    }
    
    std::string line;
    int lineNumber = 0;
    int totalAnts = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Пропускаем пустые строки и комментарии
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string typeName;
        double alpha, beta, pheromoneDeposit;
        int count;
        
        if (!(iss >> typeName >> alpha >> beta >> pheromoneDeposit >> count)) {
            throw AntAlgorithmException("Invalid format at line " + 
                                       std::to_string(lineNumber) + " in " + configFile);
        }
        
        if (count <= 0) {
            throw AntAlgorithmException("Invalid count at line " + 
                                       std::to_string(lineNumber) + " in " + configFile); 
        }

        try {
            std::cout << "Creating " << count << " ants of type '" << typeName 
                      << "' (alpha=" << alpha << ", beta=" << beta 
                      << ", pheromone=" << pheromoneDeposit << ")" << std::endl;
            
            for (int i = 0; i < count; ++i) {
                ants.emplace_back(alpha, beta, pheromoneDeposit, typeName);
            }
            totalAnts += count;
        } catch (const AntAlgorithmException& e) {
            throw AntAlgorithmException("Error creating ants at line " + 
                                       std::to_string(lineNumber) + ": " + e.what());
        }
    }
    
    file.close();
    
    if (totalAnts == 0) {
        throw AntAlgorithmException("No ants were created from config file");
    }
    
}

const std::vector<double>& AntColonyOptimization::getBestPathPheromoneHistory() const {
    return bestPathPheromoneHistory;
}

std::pair<double, std::vector<AntEdge*>> AntColonyOptimization::findHamiltonianCycle(Node* startNode) {
    double bestCycleLength = -1.0;
    std::vector<AntEdge*> bestCycle;
    pheromoneHistory.clear();
    bestPathPheromoneHistory.clear();
    bestPathLengthHistory.clear();
    currentPathLengthHistory.clear();
    int no_change_counter = 0;
    double previousBestLength = -1.0;

    for (int i = 0; i < iterations; ++i) {
        resetAnts(startNode);
        double currentIterationBest = -1.0;
        
        for (auto& ant : ants) {
            while (true) {
                Node* currnode = ant.getCurrentNode();
                std::vector<AntEdge>& all_neighbors = graph.getGraphNonConst().at(currnode);
                
                std::vector<AntEdge*> available_neighbors;
                for(AntEdge& edge : all_neighbors){
                    if (!ant.is_visited(edge.getEnd())){
                        available_neighbors.push_back(&edge);
                    }
                }

                if (available_neighbors.empty()){
                    break; 
                }

                AntEdge* chosen_edge = ant.chooseNextNode(available_neighbors);
                
                if (chosen_edge){
                    ant.visitNode(chosen_edge);
                } else {
                    break;
                }
            }

            // Проверяем, вернулся ли муравей в начальную точку
            if (ant.getPath().size() == graph.getGraph().size() - 1) {
                Node* lastNode = ant.getCurrentNode();
                AntEdge* closingEdge = graph.findEdge(lastNode, startNode);
                if (closingEdge) {
                    ant.visitNode(closingEdge); // Замыкаем цикл
                    double cycleLength = ant.getPathLength();
                    if (currentIterationBest < 0 || cycleLength < currentIterationBest) {
                        currentIterationBest = cycleLength;
                    }
                    if (bestCycleLength < 0 || cycleLength < bestCycleLength) {
                        bestCycleLength = cycleLength;
                        bestCycle = ant.getPath();
                    }
                }
            }
        }
        
        recordBestPathLength(bestCycleLength);
        recordCurrentPathLength(currentIterationBest > 0 ? currentIterationBest : bestCycleLength);
        recordBestPathPheromone(bestCycle);
        
        for (auto& pair : graph.getGraphNonConst()) {
            for (auto& edge : pair.second) {
                double currentpheromone = edge.getPheromone();
                edge.setPheromone(currentpheromone * (1.0 - evaporationRate));
            }
        }
        for (const auto& ant : ants) {
             if (ant.getPath().size() == graph.getGraph().size()) {
                const auto& path = ant.getPath();
                double pathLength = ant.getPathLength();
                if (pathLength > 0) {
                    double deltaPheromone = ant.getPheromoneDeposit() / pathLength;
                    for (AntEdge* edge : path) {
                        double currentPheromone = edge->getPheromone();
                        edge->setPheromone(currentPheromone + deltaPheromone);
                    }
                }
            }
        }
        recordPheromoneState();
        
        if (i > 0 && previousBestLength > 0) {
            if (std::abs(bestCycleLength - previousBestLength) < 1e-9) {
                no_change_counter++;
            } else {
                no_change_counter = 0;
            }
        }
        previousBestLength = bestCycleLength;

        if (no_change_counter >= stagnation_limit) {
            std::cout << "\nAlgorithm stopped: best path hasn't improved for " << stagnation_limit << " iterations (stopped at iteration " << i + 1 << ")." << std::endl;
            break;
        }
    }

    return {bestCycleLength, bestCycle};
}
