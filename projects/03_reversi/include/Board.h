#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <string>

struct Cell
{
  Cell(std::string y_str, int x, int value);
  Cell(int y, int x, int value);
  int to_number(std::string y);
  int x;
  int y;
  int value;
};

class Board{
  std::vector<std::vector<Cell*>> board;
  
public:
  Board();
  Board(const Board& other); // Copy constructor
  ~Board(); // Destructor
  
  int get_value(int y, int x) const;
  void set_value(int y, int x, int value);
  int get_board_size() const;
//  Cell& get_cell_ref(int y, int x);
//  const Cell& get_cell_ref(int y, int x) const;
  
  bool is_cell_empty(int y, int x) const;
  bool is_valid_coord(int y, int x) const;
  bool is_full() const;
  
  int count_cells(int value) const;
  
  void print() const;
  void print_with_moves(const std::vector<Cell>& valid_moves) const;
};

#endif
