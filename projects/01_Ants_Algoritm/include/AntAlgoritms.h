#ifndef ANT_ALGORITHMS_H
#define ANT_ALGORITHMS_H

#include "../../../libs/cpp_graph/include/Edge.h"
#include "../../../libs/cpp_graph/include/Graph.h"
#include "../../../libs/cpp_graph/include/Node.h"
#include "Ant.h"
#include <set>
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>

class AntAlgorithmException : public std::runtime_error {
public:
  explicit AntAlgorithmException(const std::string& message);
};

class AntColonyOptimization{
  Graph<AntEdge>& graph;
  std::vector<Ant> ants;

  double evaporationRate;
  int iterations;
  int stagnation_limit;

  std::vector<double> pheromoneHistory;
  std::vector<double> bestPathPheromoneHistory;
  std::vector<double> bestPathLengthHistory;
  std::vector<double> currentPathLengthHistory;
  
  void runIteration(Node* startNode, Node* endNode);
  void updatePheromones(Node* endNode);
  void resetAnts(Node* startNode);
  void recordPheromoneState();
  void recordBestPathPheromone(const std::vector<Node*>& path);
  void recordBestPathPheromone(const std::vector<AntEdge*>& path);
  void recordBestPathLength(double bestLength);
  void recordCurrentPathLength(double currentLength);

  void loadAntsFromConfig(const std::string& configFileName);
  void loadAlgorithmParameters(const std::string& configFileName);

  void validateAlgorithmParameters() const;

public:
  AntColonyOptimization(Graph<AntEdge>& g, 
                        const std::string& antConfigFile,
                        const std::string& algConfigFile);
  const std::vector<double>& getPheromoneHistory() const;
  const std::vector<double>& getBestPathPheromoneHistory() const;
  const std::vector<double>& getBestPathLengthHistory() const;
  const std::vector<double>& getCurrentPathLengthHistory() const;

  double getEvaporationRate() const;
  int getIterations() const;
  int getStagnationLimit() const;

  //Для поиска кратчайшего пути
  std::pair<double, std::vector<Node*>> findShortestPath(Node* startNode, Node* endNode); 
  //Поиск гамильного цикла
  std::pair<double, std::vector<AntEdge*>> findHamiltonianCycle(Node* startNode);
};

#endif
