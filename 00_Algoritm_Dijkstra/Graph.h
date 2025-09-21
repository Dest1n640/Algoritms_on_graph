#ifndef GRAPH_H
#define GRAPH_H
#include <set>
#include <ostream>
#include "Node.h"

class Graph{
private:
  std::set<Node*> nodes;
public:
  typedef std::set<Node*>::const_iterator graph_iterator;
  void addNode(Node* node);
  void removeNode(Node* node);
  void addEdge(Node* begin, Node* end, int weight);   //Вот тут будет создаваться ориентированость графа
  void removeEdge(Node* begin, Node* end); //По условию задачи граф должен будет ориентированный в обе стороны
  bool is_exist(Node* begin, Node* end);  //Проверка на то существует нода в моем множестве
  graph_iterator begin() const;
  graph_iterator end() const;
};

std::ostream& operator << (std::ostream& os, const Graph& graph);


#endif
