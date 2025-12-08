#ifndef PLAYER_H
#define PLAYER_H

#include "GameEngine.h"

enum class PlayerType {
    HUMAN,
    AI
};

class Player {
protected:
    PlayerColor color;
    
public:
    Player(PlayerColor color) : color(color) {}
    virtual ~Player() = default;
    
    // Get next move from player (human input or AI calculation)
    virtual Cell get_move(GameEngine& engine) = 0;
    
    PlayerColor get_color() const { return color; }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(PlayerColor color) : Player(color) {}
    
    // Get move from human player via console input
    Cell get_move(GameEngine& engine) override;
};

class AIPlayer : public Player {
    int depth;
    
    // Minimax algorithm with alpha-beta pruning
    int minimax(GameEngine& engine, int depth, int alpha, int beta, bool maximizing);
    
    // Evaluate board position
    int evaluate_board(const GameEngine& engine);
    
public:
    AIPlayer(PlayerColor color, int depth = 7) : Player(color), depth(depth) {}
    
    // Get best move using minimax algorithm
    Cell get_move(GameEngine& engine) override;
};

#endif
