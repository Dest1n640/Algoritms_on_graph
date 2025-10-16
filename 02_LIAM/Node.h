#ifndef NODE_H
#define NODE_H

#include <cmath>

struct Node{
  int x, y;
  double gCost; //Стоимости пути от старта
  double hCost; //Эвристическая стоимость до цели
  Node* parents; //Родитель текущей ноды
  double arrivalAngle; // Угол, под которым пришли в этот узел
public:
  Node(int x, int y);
  double getFCost() const; //Полная стоимость пути
  bool operator > (const Node* other) const; //Оператор для сравнения в очереди
};

#endif
