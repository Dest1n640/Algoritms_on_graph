#ifndef ANT_ALGORITHMS_H
#define ANT_ALGORITHMS_H

#include "Graph.h"
#include "Edge.h"
#include <set>
#include <vector>

extern double alpha;
extern double beta;
extern double evaporationRate; //p
extern double pheromoneDeposit; //triangle


class Ant{
  std::vector<AntEdge*> path; 
  std::set<Node*> visited;
  Node* currentNode;
  double pathLength;
  
public:
  Ant();
  AntEdge* chooseNextNode(const std::vector<AntEdge*>& neighbors);
  bool reached(Node* target);
  bool is_visited(Node* nextNode);
  void visitNode(AntEdge* edge);
  void reset(Node* startNode);

  const std::vector<AntEdge*>& getPath() const;
  double getPathLength() const;
  Node* getCurrentNode() const;
};

class AntColonyOptimization{
  Graph<AntEdge>& graph;
  std::vector<Ant> ants;
  
  void runIteration(Node* startNode, Node* endNode);
  void updatePheromones(Node* endNode);
  void resetAnts(Node* startNode);

public:
  AntColonyOptimization(Graph<AntEdge>& g, int numAnts);
  double run(Node* startNode, Node* endNode, int iterations);
};

#endif
