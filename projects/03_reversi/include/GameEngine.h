#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include "Board.h"

enum class PlayerColor {
    BLACK = 1,
    WHITE = -1,
    NONE = 0
};

enum class GameStatus {
    PLAYING,
    BLACK_WINS,
    WHITE_WINS,
    DRAW
};

class Player;

class GameEngine{
private:
  Board* board;
  PlayerColor current_player;
  int black_score;
  int white_score;

  std::vector<Cell> get_flipped_cells(Cell coord, PlayerColor player) const;
  bool is_valid_direction(int y, int x, int dy, int dx, PlayerColor player, std::vector<Cell>& to_flip);
  std::vector<Cell> get_valid_moves(PlayerColor player) const;
  void update_scores();

public:
  GameEngine();
  GameEngine(const GameEngine& other);
  ~GameEngine();
  
  // Make a move at the given coordinate
  bool make_move(Cell coord);
  
  // Switch to the next player
  void switch_player();
  
  // Check if player has valid moves
  bool has_valid_moves(PlayerColor player) const;
  
  // Get all possible moves for current player
  std::vector<Cell> get_possible_moves() const;

  // Get current game status
  GameStatus get_game_status();
  
  // Get current player
  PlayerColor get_current_player() const;

  // Get scores
  int get_black_score() const;
  int get_white_score() const;
  
  // Get board reference
  const Board& get_board() const;
  
  // Main game loop - runs the entire game
  void run_game(Player* player1, Player* player2);
};
#endif
