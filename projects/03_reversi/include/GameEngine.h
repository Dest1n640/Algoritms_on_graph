#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include "Board.h"

enum class Player {
    BLACK = 1,
    WHITE = -1,
    NONE = 0
};

enum class GameStatus {
    PLAYING,
    BLACK_WON,
    WHITE_WON,
    DRAW
};


class GameEngine{
private:
  Board* board;
  Player current_player;
  int black_score;
  int white_score;

  std::vector<Cell> get_flipped_cells(Cell coord, Player player) const;
  bool is_valid_direction(int y, int x, int dy, int dx, Player player, std::vector<Cell>& to_flip);
  std::vector<Cell> get_valid_moves(Player player) const;
  void update_scores();


public:
  GameEngine();
  GameEngine(const GameEngine& other); // Copy constructor
  ~GameEngine(); // Destructor
  bool make_move(Cell coord);
  void switch_player();
  bool has_valid_moves(Player player);
  std::vector<Cell> get_possible_moves();  // Для текущего игрока

  GameStatus get_game_status();
  Player get_current_player() const;

  int get_black_score() const;
  int get_white_score() const;
  const Board& get_board() const;

  void display_board() const;
  void display_board_with_moves() const;
  std::string board_to_string() const;
};
#endif
