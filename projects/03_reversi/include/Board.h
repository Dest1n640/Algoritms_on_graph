#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <string>

struct Cell
{
  int x;
  int y;
  int value;
  
  Cell(int x, int y, int value) : x(x), y(y), value(value) {}
};

class Board{
  std::vector<std::vector<Cell*>> board;
  
public:
  Board();
  Board(const Board& other);
  ~Board();
  
  int get_value(int y, int x) const;
  void set_value(int y, int x, int value);
  int get_board_size() const;
  
  bool is_cell_empty(int y, int x) const;
  bool is_valid_coord(int y, int x) const;
  bool is_full() const;
  
  int count_cells(int value) const;
  
  void print(const std::vector<Cell>& valid_moves = std::vector<Cell>()) const;
};

#endif
