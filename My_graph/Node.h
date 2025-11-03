#ifndef NODE_H
#define NODE_H
#include <string>
#include <utility> // Для std::pair

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

class NodeLian : public Node{
  int x, y;
public:
  NodeLian(const std::string& n, int xn, int yn) : Node(n), x(xn), y(yn) {}
  NodeLian(int xn, int yn) : Node(""), x(xn), y(yn) {} // Вспомогательный конструктор

  int getX() const { return x; }
  int getY() const { return y; }
  std::pair<int, int> getXY() const { return {x, y}; }

  // Оператор < для использования в std::map/std::set
  bool operator<(const NodeLian& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
  }

  // Оператор == для сравнения узлов
  bool operator==(const NodeLian& other) const {
    return x == other.x && y == other.y;
  }
};

#endif
