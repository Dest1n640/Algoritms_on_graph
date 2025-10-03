#ifndef EDGE_H
#define EDGE_H
#include <string>
#include "Node.h"

class Edge{
  Node* begin;
  Node* end;
  int weight;
public:
  Edge(Node* b, Node* e, int w);
  Node* getBegin() const;
  Node* getEnd() const;
  int getWeight() const;
  friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
};

class AntEdge : public Edge{
  double pheromone;
public:
  AntEdge(Node* b, Node* e, int w, double p = 1.0);
  double getPheromone() const;
  void setPheromone(double p);
  friend std::ostream& operator<<(std::ostream& os, const AntEdge& edge);
};

#endif
