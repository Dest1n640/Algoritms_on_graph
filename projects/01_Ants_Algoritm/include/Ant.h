#ifndef ANT_H
#define ANT_H

#include "../../../libs/cpp_graph/include/Node.h"
#include "../../../libs/cpp_graph/include/Edge.h"
#include <vector>
#include <set>
#include <string>
#include <exception>
#include <stdexcept>

class AntException : public std::runtime_error {
public:
  explicit AntException(const std::string& message);
};


class Ant{
  std::vector<AntEdge*> path;  
  std::set<Node*> visited;
  Node* currentNode;
  double pathLength;

  double alpha;
  double beta;
  double pheromoneDeposit;
  std::string typeName;
  
public:
  Ant();
  Ant(double alpha, double beta, double pheromoneDeposit, const std::string& typeName);
  AntEdge* chooseNextNode(const std::vector<AntEdge*>& neighbors); 
  bool reached(Node* target);
  bool is_visited(Node* nextNode);
  void visitNode(AntEdge* edge);
  void reset(Node* startNode);

  const std::vector<AntEdge*>& getPath() const;
  double getPathLength() const;
  Node* getCurrentNode() const;
  double getPheromoneDeposit() const;
  const std::string& getTypeName() const;

  static void validateParameters(double alpha, double beta, double pheromoneDeposit);
};


#endif
