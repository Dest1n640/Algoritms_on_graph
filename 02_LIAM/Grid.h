#ifndef GRID_H
#define GRID_H
#include <vector>
#include <string>
#include "Node.h"

class Grid{
  int width;
  int height;
  Node* start;
  Node* end;
  std::vector<std::vector<bool>> obstacle;
  // Парсинг BMP файла
  void loadFromBmp(const std::string FileName);
  void updateStart(int xs, int ys);
  void updateEnd(int xe, int ye);
public:
  Grid(const std::string& bmpFileName);
  ~Grid();
  // Является ли ячейка препятствием
  bool isObstacle(int x, int y) const;
  int getWidth() const;
  int getHeight() const;
  void operator>>(const Grid& other)const;
};
#endif
