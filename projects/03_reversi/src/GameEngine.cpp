#include "../include/GameEngine.h"
#include "../include/Player.h"
#include "../include/Display.h"
#include <iostream>

GameEngine::GameEngine() {
  board = new Board();
  current_player = PlayerColor::BLACK; 
  black_score = 2;
  white_score = 2;
}

GameEngine::GameEngine(const GameEngine& other) {
  board = new Board(*other.board);
  current_player = other.current_player;
  black_score = other.black_score;
  white_score = other.white_score;
}

GameEngine::~GameEngine() {
  delete board;
}

bool GameEngine::make_move(Cell coord){
  int y = coord.y;
  int x = coord.x;
  
  if (!board->is_valid_coord(y, x) || !board->is_cell_empty(y, x)) {
    return false;
  }

  PlayerColor my_player = get_current_player();
  std::vector<Cell> flipped = get_flipped_cells(coord, my_player);
  
  if (flipped.empty()) {
    return false;
  }

  board->set_value(y, x, static_cast<int>(my_player));
  
  for (const auto& cell : flipped) {
    board->set_value(cell.y, cell.x, static_cast<int>(my_player));
  }
  
  update_scores();
  return true;
}

std::vector<Cell> GameEngine::get_flipped_cells(Cell coord, PlayerColor player) const{
  std::vector<Cell> flipped_cells;
  int y = coord.y;
  int x = coord.x;
  
  if (board->get_value(y, x) != 0) {
    return flipped_cells;
  }
  
  int directions[8][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
  };
  
  int player_value = static_cast<int>(player);
  int opponent_value = (player == PlayerColor::BLACK) ? static_cast<int>(PlayerColor::WHITE) : static_cast<int>(PlayerColor::BLACK);
  
  for (int dir = 0; dir < 8; dir++) {
    std::vector<Cell> temp_flipped;
    int dy = directions[dir][0];
    int dx = directions[dir][1];
    int curr_y = y + dy;
    int curr_x = x + dx;
    
    while (board->is_valid_coord(curr_y, curr_x) && 
           board->get_value(curr_y, curr_x) == opponent_value) {
      temp_flipped.push_back(Cell(curr_y, curr_x, opponent_value));
      curr_y += dy;
      curr_x += dx;
    }
    
    if (board->is_valid_coord(curr_y, curr_x) && 
        board->get_value(curr_y, curr_x) == player_value && 
        !temp_flipped.empty()) {
      flipped_cells.insert(flipped_cells.end(), temp_flipped.begin(), temp_flipped.end());
    }
  }
  
  return flipped_cells;
}

bool GameEngine::is_valid_direction(int y, int x, int dy, int dx, PlayerColor player, std::vector<Cell>& to_flip){
  int player_value = static_cast<int>(player);
  int opponent_value = (player == PlayerColor::BLACK) ? static_cast<int>(PlayerColor::WHITE) : static_cast<int>(PlayerColor::BLACK);
  
  int curr_y = y + dy;
  int curr_x = x + dx;
  
  std::vector<Cell> temp_flip;
  
  while (board->is_valid_coord(curr_y, curr_x) && 
         board->get_value(curr_y, curr_x) == opponent_value) {
    temp_flip.push_back(Cell(curr_y, curr_x, opponent_value));
    curr_y += dy;
    curr_x += dx;
  }
  
  if (!temp_flip.empty() && 
      board->is_valid_coord(curr_y, curr_x) && 
      board->get_value(curr_y, curr_x) == player_value) {
    to_flip.insert(to_flip.end(), temp_flip.begin(), temp_flip.end());
    return true;
  }
  
  return false;
}

std::vector<Cell> GameEngine::get_valid_moves(PlayerColor player) const{
  std::vector<Cell> valid_moves;
  
  for(int i = 0; i < board->get_board_size(); i++){
    for (int j = 0; j < board->get_board_size(); j++){
      if(board->get_value(i, j) == 0) {
        Cell test_coord(i, j, 0);
        std::vector<Cell> flipped = get_flipped_cells(test_coord, player);
        
        if(!flipped.empty()) {
          valid_moves.push_back(test_coord);
        }
      }
    }
  }
  
  return valid_moves;
}

void GameEngine::update_scores(){
  white_score = 0;
  black_score = 0;
  for (int i = 0; i < board->get_board_size(); i++){
    for (int j = 0; j < board->get_board_size(); j++){
      if(board->get_value(i, j) == 1)
        black_score++;
      else if(board->get_value(i, j) == -1)
        white_score++;
    }
  }
}

void GameEngine::switch_player(){
  if (current_player == PlayerColor::BLACK) {
    current_player = PlayerColor::WHITE;
  } else {
    current_player = PlayerColor::BLACK;
  }
}

bool GameEngine::has_valid_moves(PlayerColor player) const{
  std::vector<Cell> valid_moves = get_valid_moves(player);
  return !valid_moves.empty();
}

std::vector<Cell> GameEngine::get_possible_moves() const{
  return get_valid_moves(current_player);
}

GameStatus GameEngine::get_game_status(){
  bool game_over = board->is_full() || 
                   (!has_valid_moves(PlayerColor::BLACK) && !has_valid_moves(PlayerColor::WHITE));
  
  if (!game_over) {
    return GameStatus::PLAYING;
  }
  
  update_scores();
  
  if (black_score > white_score) {
    return GameStatus::BLACK_WINS;
  } else if (white_score > black_score) {
    return GameStatus::WHITE_WINS;
  } else {
    return GameStatus::DRAW;
  }
}

PlayerColor GameEngine::get_current_player() const{
  return current_player;
}

int GameEngine::get_black_score() const{
  return black_score;
}

int GameEngine::get_white_score() const{
  return white_score;
}

const Board& GameEngine::get_board() const{
  return *board;
}

void GameEngine::run_game(Player* player1, Player* player2) {
    int consecutive_passes = 0;
    
    while (get_game_status() == GameStatus::PLAYING) {
        Display::print_board(*this, true);
        Display::print_scores(black_score, white_score);
        
        Player* current = (current_player == PlayerColor::BLACK) ? player1 : player2;
        
        if (!has_valid_moves(current_player)) {
            Display::print_skip_turn(current_player);
            switch_player();
            consecutive_passes++;
            
            if (consecutive_passes >= 2) {
                break;
            }
            continue;
        }
        
        consecutive_passes = 0;
        
        Cell move = current->get_move(*this);
        
        if (!make_move(move)) {
            Display::print_invalid_move();
            continue;
        }
        
        Display::print_player_move(current_player, move.y, move.x);
        switch_player();
    }
    
    Display::print_board(*this, false);
    Display::print_game_over(get_game_status(), black_score, white_score);
}
