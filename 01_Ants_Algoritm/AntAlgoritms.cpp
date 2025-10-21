#include "AntAlgoritms.h"
#include "../My_graph/Edge.h"
#include "../My_graph/Node.h"
#include <random>
#include <math.h>

double alpha = 1.0;
double beta = 1.0;
double evaporationRate = 0.5;
double pheromoneDeposit = 1.0;

Ant :: Ant() : currentNode(nullptr), pathLength(0.0) {}

Node* Ant::getCurrentNode() const{
  return currentNode;
} 
double Ant::getPathLength() const{
  return pathLength;
}
const std::vector<AntEdge*>& Ant::getPath() const{
  return path;
}


bool Ant::reached(Node* target){
  return currentNode == target;
}

AntEdge* Ant::chooseNextNode(const std::vector<AntEdge*>& neighbors){
  double denominator = 0.0;
  std::vector<double> probabilities;
  probabilities.reserve(neighbors.size());

  for (const auto& edge : neighbors) {
      double t = edge->getPheromone();
      double n = 1.0 / edge->getWeight();
      probabilities.push_back(std::pow(t, alpha) * std::pow(n, beta));
      denominator += probabilities.back();
  }

  if (denominator == 0.0) {
      return nullptr;
  }

  for (double& p : probabilities) {
      p /= denominator;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);
  double r = dis(gen);
  double cumulative = 0;
  for (size_t i = 0; i < probabilities.size(); i++) {
      cumulative += probabilities[i];
      if (r <= cumulative) {
          return neighbors[i]; // Возвращаем указатель на оригинальное ребро
      }
  }
  return neighbors.back();
}

bool Ant::is_visited(Node* nextNode){
  return visited.count(nextNode) > 0;
}

void Ant::visitNode(AntEdge* edge){
  path.push_back(edge);
  visited.insert(edge->getEnd());
  currentNode = edge->getEnd();
  pathLength += edge->getWeight();
}

void Ant::reset(Node* startNode){
  currentNode = startNode;
  path.clear();
  visited.clear();
  if (startNode) {
      visited.insert(startNode);
  }
  pathLength = 0;
}

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
            double deltaPheromone = pheromoneDeposit / pathLength;
            for (AntEdge* edge : path) {
                double currentPheromone = edge->getPheromone();
                edge->setPheromone(currentPheromone + deltaPheromone);
            }
        }
    }
}
AntColonyOptimization::AntColonyOptimization(Graph <AntEdge>& g, int numAnts): graph(g) {
  ants.resize(numAnts);
};

double AntColonyOptimization::run(Node* startNode, Node* endNode, int iterations) {
    double bestPathLength = -1.0;

    for (int i = 0; i < iterations; ++i) {
        // 1. Запускаем всех муравьев для построения путей
        runIteration(startNode, endNode);

        // 2. Обновляем феромоны на графе
        updatePheromones(endNode);

        // 3. Ищем лучший путь найденный в этой итерации
        for (const auto& ant : ants) {
            if (ant.getCurrentNode() == endNode) {
                double currentPathLength = ant.getPathLength();
                if (bestPathLength == -1.0 || currentPathLength < bestPathLength) {
                    bestPathLength = currentPathLength;
                }
            }
        }
    }

    return bestPathLength;
}


std::pair<double, std::vector<AntEdge*>> AntColonyOptimization::HamCycle(Node* startNode, int iterations){
    double bestCycleLength = -1.0;
    std::vector<AntEdge*> bestPath;
    const size_t totalNodes = graph.getGraph().size();

    for (int i = 0; i < iterations; ++i) {
        resetAnts(startNode);

        for (auto& ant : ants) {
            while (ant.getPath().size() < totalNodes - 1) {
                Node* currnode = ant.getCurrentNode();
                std::vector<AntEdge>& all_neighbors = graph.getGraphNonConst().at(currnode);
                
                std::vector<AntEdge*> available_neighbors;
                for (AntEdge& edge : all_neighbors) {
                    if (!ant.is_visited(edge.getEnd())) {
                        available_neighbors.push_back(&edge);
                    }
                }

                if (available_neighbors.empty()) {
                    break;
                }

                AntEdge* chosen_edge = ant.chooseNextNode(available_neighbors);
                if (chosen_edge) {
                    ant.visitNode(chosen_edge);
                } else {
                    break;
                }
            }

            if (ant.getPath().size() == totalNodes - 1) {
                Node* lastNode = ant.getCurrentNode();
                const auto& neighbors = graph.getNeighbors(lastNode);
                for (const auto& edge : neighbors) {
                    if (edge.getEnd() == startNode) {
                        double currentCycleLength = ant.getPathLength() + edge.getWeight();
                        if (bestCycleLength == -1.0 || currentCycleLength < bestCycleLength) {
                            bestCycleLength = currentCycleLength;
                            bestPath = ant.getPath();
                        }
                        break;
                    }
                }
            }
        }
        
        updatePheromones(startNode); 
    }

    return {bestCycleLength, bestPath};
}
