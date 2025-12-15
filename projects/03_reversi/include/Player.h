#ifndef PLAYER_H
#define PLAYER_H

#include "GameEngine.h"
#include <thread>
#include <mutex>
#include <vector>

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
    
    // Structure to store search result for one move
    struct SearchResult {
        Cell move;
        int score;
        
        SearchResult() : move(-1, -1, 0), score(std::numeric_limits<int>::min()) {}
        SearchResult(Cell m, int s) : move(m), score(s) {}
    };
    
    // Minimax algorithm with alpha-beta pruning
    int minimax(GameEngine& engine, int depth, int alpha, int beta, bool maximizing);
    
    // Evaluate board position
    int evaluate_board(const GameEngine& engine);

    // Helper functions for evaluation
    int get_position_priority(int y, int x) const;
    int calculate_edge_stability(const Board& board, int corner_y, int corner_x, int player_color) const;
    
    // Parallel search for one move (executed in separate thread)
    void search_move(const GameEngine& engine, const Cell& move, int depth, SearchResult& result);
    
public:
    AIPlayer(PlayerColor color, int depth = 10) : Player(color), depth(depth) {}
    
    // Get best move using minimax algorithm
    Cell get_move(GameEngine& engine) override;
};

#endif
