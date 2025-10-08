#ifndef NODE_H
#define NODE_H
#include <string>

// Нодой является просто объект, который хранит какое то название

class Node{
  std::string name;
  public:
  Node(std::string n) : name(n) {}
  std::string getName() const { return name; }

  bool operator<(const Node& other) const { 
    return name < other.name;
  } // Оператор который позволяет упорядочить объекты класса

};

#endif
