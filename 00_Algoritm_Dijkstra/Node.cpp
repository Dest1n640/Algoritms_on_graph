#include <vector>
#include <string>
#include <set>
#include "Node.h"

void Node::addNeighbours(Node* neighbour, int weight){
  neighbours[neighbour] =  weight;
}
void Node::removeNeighbours(Node* neighbour){
  neighbours.erase(neighbour);
}

Node::Node(const std::string aname) : name(aname) {}

Node::Node(const int aname) : int_name(aname){}

const std::string Node::getName() const{
  return name;
}

Node::node_iterator Node::nb_begin() const {
  return neighbours.begin();
}
Node::node_iterator Node::nb_end() const {
  return neighbours.end();
}
