#include "Graph.h"
#include <iostream>

int main(){
  Graph <AntEdge> graph;

  graph.addNode("1");
  graph.addNode("2");
  graph.addEdge("1", "2", 10);

  std::cout << graph;

  return 0;
}
