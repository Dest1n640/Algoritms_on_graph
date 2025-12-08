#include "../include/Player.h"
#include "../include/Display.h"
#include <limits>
#include <iostream>

Cell HumanPlayer::get_move(GameEngine& engine) {
    while (true) {
        std::string input = Display::get_player_input();
        
        int row, col;
        if (!Display::parse_move(input, row, col)) {
            Display::print_invalid_format();
            continue;
        }
        
        Cell move(col, row, 0);
        
        std::vector<Cell> valid_moves = engine.get_possible_moves();
        bool is_valid = false;
        
        for (const auto& valid_move : valid_moves) {
            if (valid_move.x == move.x && valid_move.y == move.y) {
                is_valid = true;
                break;
            }
        }
        
        if (is_valid) {
            return move;
        } else {
            Display::print_invalid_move();
        }
    }
}

Cell AIPlayer::get_move(GameEngine& engine) {
    int best_score = std::numeric_limits<int>::min();
    Cell best_move(-1, -1, 0);
    
    std::vector<Cell> possible_moves = engine.get_possible_moves();
    
    if (possible_moves.empty()) {
        return best_move;
    }

    for (const auto& move : possible_moves) {
        GameEngine temp_engine = engine;
        temp_engine.make_move(move);
        temp_engine.switch_player();
        
        int score = minimax(temp_engine, depth - 1, 
                           std::numeric_limits<int>::min(), 
                           std::numeric_limits<int>::max(), false);
        
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }
    
    return best_move;
}

int AIPlayer::minimax(GameEngine& engine, int depth, int alpha, int beta, bool maximizing) {
    if (depth == 0 || engine.get_game_status() != GameStatus::PLAYING) {
        return evaluate_board(engine);
    }

    std::vector<Cell> possible_moves = engine.get_possible_moves();

    if (possible_moves.empty()) {
        engine.switch_player();
        int result = minimax(engine, depth - 1, alpha, beta, !maximizing);
        engine.switch_player();
        return result;
    }

    if (maximizing) {
        int max_eval = std::numeric_limits<int>::min();
        for (const auto& move : possible_moves) {
            GameEngine temp_engine = engine;
            temp_engine.make_move(move);
            temp_engine.switch_player();
            int eval = minimax(temp_engine, depth - 1, alpha, beta, false);
            max_eval = std::max(max_eval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return max_eval;
    } else {
        int min_eval = std::numeric_limits<int>::max();
        for (const auto& move : possible_moves) {
            GameEngine temp_engine = engine;
            temp_engine.make_move(move);
            temp_engine.switch_player();
            int eval = minimax(temp_engine, depth - 1, alpha, beta, true);
            min_eval = std::min(min_eval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return min_eval;
    }
}

int AIPlayer::evaluate_board(const GameEngine& engine) {
    if (color == PlayerColor::BLACK) {
        return engine.get_black_score() - engine.get_white_score();
    } else {
        return engine.get_white_score() - engine.get_black_score();
    }
}
