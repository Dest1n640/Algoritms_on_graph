#ifndef NODE_H
#define NODE_H
#include <string>

class Node{
  std::string name;
  public:
  Node(std::string n) : name(n) {}
  std::string getName() const { return name; }

  bool operator<(const Node& other) const {
    return name < other.name;
  }

};

#endif
