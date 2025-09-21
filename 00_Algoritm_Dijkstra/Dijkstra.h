#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <vector>
#include "Node.h"
#include "Graph.h"
#include "PriorityQueue.h"

struct  Way
{
  std::vector<Node*> nodes;
  int length;
  Way() : length(-1){}
};

class Dijkstra{
  const Graph& graph;
public:
  Dijkstra(const Graph& agraph);
  Way shortestWay(Node* begin, Node* end);
  static Way unroll(const std::map<Node*, MarkedNode>& visited, Node* begin, Node* curr);
};


#endif
