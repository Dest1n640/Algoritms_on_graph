#include "../include/AI.h"
#include <algorithm>
#include <iostream>

AI::AI(Player ai_player, int depth) : ai_player(ai_player), max_depth(depth) {}

Cell AI::get_best_move(GameEngine& engine) {
    int best_score = std::numeric_limits<int>::min();
    Cell best_move(-1, -1, 0);
    
    std::vector<Cell> possible_moves = engine.get_possible_moves();
    
    if (possible_moves.empty()) {
        return best_move; 
    }

    if (possible_moves.size() == 1) {
        return possible_moves[0];
    }

    for (const auto& move : possible_moves) {
        GameEngine temp_engine = engine;
        temp_engine.make_move(move);
        temp_engine.switch_player();
        
        int score = minimax(temp_engine, max_depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
        
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }
    
    return best_move;
}

int AI::minimax(GameEngine engine, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || engine.get_game_status() != GameStatus::PLAYING) {
        return evaluate(engine);
    }

    std::vector<Cell> moves = engine.get_possible_moves();
    
    if (moves.empty()) {
        // Pass turn
        engine.switch_player();
        return minimax(engine, depth, alpha, beta, !maximizingPlayer);
    }

    if (maximizingPlayer) {
        int max_eval = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            GameEngine temp_engine = engine;
            temp_engine.make_move(move);
            temp_engine.switch_player();
            int eval = minimax(temp_engine, depth - 1, alpha, beta, false);
            max_eval = std::max(max_eval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return max_eval;
    } else {
        int min_eval = std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            GameEngine temp_engine = engine;
            temp_engine.make_move(move);
            temp_engine.switch_player();
            int eval = minimax(temp_engine, depth - 1, alpha, beta, true);
            min_eval = std::min(min_eval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return min_eval;
    }
}

int AI::evaluate(const GameEngine& engine) {
    int my_score = 0;
    int op_score = 0;
    
    if (ai_player == Player::BLACK) {
        my_score = engine.get_black_score();
        op_score = engine.get_white_score();
    } else {
        my_score = engine.get_white_score();
        op_score = engine.get_black_score();
    }
    
    int score = (my_score - op_score);
    
    const Board& board = engine.get_board();
    int corners[4][2] = {{0,0}, {0,7}, {7,0}, {7,7}};
    int corner_value = 30;
    
    for(auto& c : corners) {
        int val = board.get_value(c[0], c[1]);
        if (val == static_cast<int>(ai_player)) {
            score += corner_value;
        } else if (val != 0) {
            score -= corner_value;
        }
    }
    
    return score;
}
