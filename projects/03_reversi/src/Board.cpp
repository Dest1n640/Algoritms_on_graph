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
    return c - 'a' + 1;
  if (c >= 'A' && c <= 'H')
    return c - 'A' + 1;
  return -1;
}

//Класс Board

Board::Board(){
  board.resize(8, std::vector<Cell*>(8, nullptr));
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      board[i][j] = new Cell(i, j, 0);
    }
  }
  board[start_black_coord1.y][start_black_coord1.x] = &start_black_coord1;
  board[start_black_coord2.y][start_black_coord2.x] = &start_black_coord2;
  board[start_white_coord1.y][start_white_coord1.x] = &start_white_coord1;
  board[start_white_coord2.y][start_white_coord2.x] = &start_white_coord2;
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
  return board.size()
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
  if (board[y][x] -> value == 0 && x || y <= 8 && x || y >= 1){
    return true;
  }
  return false;
}

int Board::count_cells(int value) const{
  int count = 0;
  for(int i = 0; i < board.size(); i++){
    for (int j = 0; j < board[i].size(); j++){
      if(board[i][j] -> value == value)
        count++;
    }
  }
  return count;
}

void Board::print() const{
  std::string line = "-";
  for (int i = 0; i < board.size(); i++){
    std::cout << line;
  }
  std::cout << std::endl;
  for (int i = 0; i < board.size(); i++){
    for (int j = 0; i < board.size(); j++){
      if (j == 0){
        std::cout << "|" << std::endl;
      }
      std::cout << board[i][j]->value;
      if (j == 8)
        std::cout << "|" << std::endl;
    }
  }
  for(int i = 0; i < board.size(); i++){
    std::cout << line;
  }
  std::cout << std::endl;
}
