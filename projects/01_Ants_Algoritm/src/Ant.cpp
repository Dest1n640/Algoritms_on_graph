#include "../include/Ant.h"
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

AntException::AntException(const std::string& message)
    : std::runtime_error(message) {}

Ant::Ant() : currentNode(nullptr), pathLength(0.0), alpha(0), beta(0), 
          pheromoneDeposit(0.0), typeName("Default") {}
          
Ant::Ant(double alpha, double beta, double pheromoneDeposit, const std::string& typeName)
  : currentNode(nullptr), pathLength(0.0),
  alpha(alpha), beta(beta),
  pheromoneDeposit(pheromoneDeposit),
  typeName(typeName) {
    validateParameters(alpha, beta, pheromoneDeposit);}


double Ant::getPheromoneDeposit() const{
  return pheromoneDeposit;
}

const std::string& Ant::getTypeName() const{
  return typeName;
}

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
          return neighbors[i];
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
void Ant::validateParameters(double alpha, double beta, double pheromoneDeposit){
  if (alpha < 0.0) 
    throw AntException("Invalid alpha parameter");
  if (beta < 0.0)
    throw AntException("Invalid beta parameter");
  if(pheromoneDeposit <= 0.0)
    throw AntException("Invalid pheromoneDeposit parameter");
}
