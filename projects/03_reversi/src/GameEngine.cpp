#include "../include/GameEngine.h"
#include "../include/Board.h"
#include <vector>
#include <string>

bool GameEngine::make_move(Cell coord){
  int y = coord.y;
  int x = coord.x;
  
  if (!board->is_valid_coord(y, x) || !board->is_cell_empty(y, x)) {
    return false;
  }

  Player my_player = get_current_player();
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

std::vector<Cell> GameEngine::get_flipped_cells(Cell coord, Player player){
  std::vector<Cell> flipped_cells;
  int y = coord.y;
  int x = coord.x;
  
  // Проверяем, что клетка пустая
  if (board->get_value(y, x) != 0) {
    return flipped_cells;
  }
  
  // 8 направлений: вверх, вниз, влево, вправо и 4 диагонали
  int directions[8][2] = {
    {-1, 0},  // вверх
    {1, 0},   // вниз
    {0, -1},  // влево
    {0, 1},   // вправо
    {-1, -1}, // вверх-влево
    {-1, 1},  // вверх-вправо
    {1, -1},  // вниз-влево
    {1, 1}    // вниз-вправо
  };
  
  int player_value = static_cast<int>(player);
  int opponent_value = (player == Player::BLACK) ? static_cast<int>(Player::WHITE) : static_cast<int>(Player::BLACK);
  
  // Проверяем каждое направление
  for (int dir = 0; dir < 8; dir++) {
    std::vector<Cell> temp_flipped;
    int dy = directions[dir][0];
    int dx = directions[dir][1];
    int curr_y = y + dy;
    int curr_x = x + dx;
    
    // Идем в направлении, пока находим фишки противника
    while (board->is_valid_coord(curr_y, curr_x) && 
           board->get_value(curr_y, curr_x) == opponent_value) {
      temp_flipped.push_back(Cell(curr_y, curr_x, opponent_value));
      curr_y += dy;
      curr_x += dx;
    }
    
    // Если нашли свою фишку после фишек противника, добавляем их в результат
    if (board->is_valid_coord(curr_y, curr_x) && 
        board->get_value(curr_y, curr_x) == player_value && 
        !temp_flipped.empty()) {
      flipped_cells.insert(flipped_cells.end(), temp_flipped.begin(), temp_flipped.end());
    }
  }
  
  return flipped_cells;
}

bool GameEngine::is_valid_direction(int y, int x, int dy, int dx, Player player, std::vector<Cell>& to_flip){
  int player_value = static_cast<int>(player);
  int opponent_value = (player == Player::BLACK) ? static_cast<int>(Player::WHITE) : static_cast<int>(Player::BLACK);
  
  int curr_y = y + dy;
  int curr_x = x + dx;
  
  // Временный вектор для хранения фишек, которые можно перевернуть
  std::vector<Cell> temp_flip;
  
  // Идем в направлении, пока находим фишки противника
  while (board->is_valid_coord(curr_y, curr_x) && 
         board->get_value(curr_y, curr_x) == opponent_value) {
    temp_flip.push_back(Cell(curr_y, curr_x, opponent_value));
    curr_y += dy;
    curr_x += dx;
  }
  
  // Проверяем, что нашли хотя бы одну фишку противника
  // и в конце стоит наша фишка
  if (!temp_flip.empty() && 
      board->is_valid_coord(curr_y, curr_x) && 
      board->get_value(curr_y, curr_x) == player_value) {
    // Направление валидно, копируем фишки в выходной вектор
    to_flip.insert(to_flip.end(), temp_flip.begin(), temp_flip.end());
    return true;
  }
  
  return false;
}

std::vector<Cell> GameEngine::get_valid_moves(Player player){
  std::vector<Cell> valid_moves;
  
  for(int i = 0; i < board->get_board_size(); i++){
    for (int j = 0; j < board->get_board_size(); j++){
      // Проверяем, что клетка пустая
      if(board->get_value(i, j) == 0) {
        // Проверяем, можно ли сделать валидный ход в эту клетку
        Cell test_coord(i, j, 0);
        std::vector<Cell> flipped = get_flipped_cells(test_coord, player);
        
        // Если при этом ходе переворачивается хотя бы одна фишка, ход валиден
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


GameEngine::GameEngine() {
  board = new Board();
  current_player = Player::BLACK;
  black_score = 2;
  white_score = 2;
}

GameEngine::GameEngine(const GameEngine& other) {
  board = new Board(*other.board); // Deep copy of board
  current_player = other.current_player;
  black_score = other.black_score;
  white_score = other.white_score;
}

GameEngine::~GameEngine() {
  delete board;
}

void GameEngine::switch_player(){
  if (current_player == Player::BLACK) {
    current_player = Player::WHITE;
  } else {
    current_player = Player::BLACK;
  }
}

bool GameEngine::has_valid_moves(Player player){
  std::vector<Cell> valid_moves = get_valid_moves(player);
  
  return !valid_moves.empty();
}

std::vector<Cell> GameEngine::get_possible_moves(){
  return get_valid_moves(current_player);
}

GameStatus GameEngine::get_game_status(){
  bool game_over = board->is_full() || 
                   (!has_valid_moves(Player::BLACK) && !has_valid_moves(Player::WHITE));
  
  if (!game_over) {
    return GameStatus::PLAYING;
  }
  
  update_scores();
  
  if (black_score > white_score) {
    return GameStatus::BLACK_WON;
  } else if (white_score > black_score) {
    return GameStatus::WHITE_WON;
  } else {
    return GameStatus::DRAW;
  }
}

Player GameEngine::get_current_player() const{
  return current_player;
}

int GameEngine::get_black_score() const{
  return black_score;
}
int GameEngine::get_white_score() const{
  return white_score;
}

void GameEngine::display_board() const{
  board->print();
}


std::string GameEngine::board_to_string() const{
  std::string result = "  1 2 3 4 5 6 7 8\n";
  
  for (int i = 0; i < board->get_board_size(); i++) {
    result += char('a' + i);
    result += " ";
    
    for (int j = 0; j < board->get_board_size(); j++) {
      int value = board->get_value(i, j);
      
      if (value == 1) {
        result += "B ";
      } else if (value == -1) {
        result += "W ";
      } else {
        result += ". ";
      }
    }
    result += "\n";
  }
  
  return result;
}

const Board& GameEngine::get_board() const{
  return *board;
}
