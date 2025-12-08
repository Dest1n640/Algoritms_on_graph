#include "../include/Board.h"
#include <iostream>

Board::Board(){
  board.resize(8, std::vector<Cell*>(8, nullptr));
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      board[i][j] = new Cell(j, i, 0);
    }
  }
  
  board[3][3]->value = -1;
  board[3][4]->value = 1;
  board[4][3]->value = 1;
  board[4][4]->value = -1;
}

Board::Board(const Board& other) {
  board.resize(8, std::vector<Cell*>(8, nullptr));
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      board[i][j] = new Cell(j, i, other.board[i][j]->value);
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
  return board[y][x]->value == 0;
}

bool Board::is_valid_coord(int y, int x) const{
  return x >= 0 && x < 8 && y >= 0 && y < 8;
}

int Board::count_cells(int value) const{
  int count = 0;
  for(size_t i = 0; i < board.size(); i++){
    for (size_t j = 0; j < board[i].size(); j++){
      if(board[i][j]->value == value) count++;
    }
  }
  return count;
}

void Board::print(const std::vector<Cell>& valid_moves) const{
  std::cout << "\n  a b c d e f g h" << std::endl;
  
  for (size_t i = 0; i < board.size(); i++){
    std::cout << (i + 1) << " ";
    
    for (size_t j = 0; j < board.size(); j++){
      int val = board[i][j]->value;
      
      bool is_valid_move = false;
      for (const auto& move : valid_moves) {
        if (move.y == static_cast<int>(i) && move.x == static_cast<int>(j)) {
          is_valid_move = true;
          break;
        }
      }
      
      if (is_valid_move) std::cout << "*";
      else if (val == 0) std::cout << ".";
      else if (val == 1) std::cout << "B";
      else if (val == -1) std::cout << "W";
      
      if (j < board.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
  }
}
