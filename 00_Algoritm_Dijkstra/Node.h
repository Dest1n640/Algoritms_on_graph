#ifndef NODE_H
#define NODE_H
#include <vector>
#include <string>
#include <set>
#include <map>

class Node{
private:
  std::string name;
  int int_name;
  std::map<Node*, int> neighbours;
  void addNeighbours(Node* neighbour, int weight);
  void removeNeighbours(Node* neighbour);
  
public:
  typedef std::map<Node*, int>::const_iterator node_iterator;
  Node(const std::string aname);
  Node(const int aname);
  const std::string getName() const;
  node_iterator nb_begin() const;
  node_iterator nb_end() const;
  friend class Graph;
};

#endif 
