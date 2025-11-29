#include "../include/Board.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

Cell::Cell(std::string y_str, int x, int value) : x(x), y(to_number(y_str)), value(value){}
Cell::Cell(int y_num, int x, int value) : x(x), y(y_num), value(value){}

int Cell::to_number(std::string y){
  if (y.length() != 1)
    return -1;
  char c = y[0];
  if (c >= 'a' && c <= 'h')
    return c - 'a';
  if (c >= 'A' && c <= 'H')
    return c - 'A';
  return -1;
}


Board::Board(){
  board.resize(8, std::vector<Cell*>(8, nullptr));
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      board[i][j] = new Cell(i, j, 0);
    }
  }
  if(is_valid_coord(3, 3)) board[3][3]->value = -1;
  if(is_valid_coord(4, 4)) board[4][4]->value = -1;
  if(is_valid_coord(3, 4)) board[3][4]->value = 1;
  if(is_valid_coord(4, 3)) board[4][3]->value = 1;
}

Board::Board(const Board& other) {
  board.resize(8, std::vector<Cell*>(8, nullptr));
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      board[i][j] = new Cell(i, j, other.board[i][j]->value);
    }
  }
}

Board::~Board() {
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      delete board[i][j];
    }
  }
}

bool Board::is_full() const{
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(board[i][j]->value == 0)
        return false;
    }
  }
  return true;
}

int Board::get_value(int y, int x) const{
  return board[y][x]->value;
}

int Board::get_board_size() const{
  return board.size();
}

void Board::set_value(int y, int x, int value){
  board[y][x]->value = value;
}

bool Board::is_cell_empty(int y, int x) const{
  if (board[y][x] -> value == 0)
    return true;
  return false;
}

bool Board::is_valid_coord(int y, int x) const{
  if (x >= 0 && x < 8 && y >= 0 && y < 8){
    return true;
  }
  return false;
}

int Board::count_cells(int value) const{
  int count = 0;
  for(size_t i = 0; i < board.size(); i++){
    for (size_t j = 0; j < board[i].size(); j++){
      if(board[i][j] -> value == value)
        count++;
    }
  }
  return count;
}

void Board::print() const{
  std::string line = "-----------------";
  std::cout << "  0 1 2 3 4 5 6 7" << std::endl;
  std::cout << line << std::endl;
  for (size_t i = 0; i < board.size(); i++){
    std::cout << i << "|";
    for (size_t j = 0; j < board.size(); j++){
      int val = board[i][j]->value;
      if (val == 0) std::cout << ".";
      else if (val == 1) std::cout << "B";
      else if (val == -1) std::cout << "W";
      std::cout << "|";
    }
    std::cout << std::endl;
    std::cout << line << std::endl;
  }
}
