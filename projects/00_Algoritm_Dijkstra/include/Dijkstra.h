#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include "../../../libs/cpp_graph/include/Edge.h"
#include "../../../libs/cpp_graph/include/Graph.h"
#include "../../../libs/cpp_graph/include/Node.h"

struct Way {
    std::vector<Node*> nodes;
    double length = -1.0;
};

class Dijkstra{
  const Graph<Edge>& graph;

public:
  Dijkstra(const Graph<Edge>& graph);
  Way shortestWay(Node* startNode, Node* endNode);
};

#endif
