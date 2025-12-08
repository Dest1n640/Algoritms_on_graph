#ifndef DISPLAY_H
#define DISPLAY_H

#include "GameEngine.h"
#include <string>

class Display {
public:
    // Parse move input from string format (e.g., "d3")
    static bool parse_move(const std::string& input, int& row, int& col);
    
    // Get player input from console
    static std::string get_player_input();
    
    // Print the board (optionally with valid moves marked)
    static void print_board(const GameEngine& engine, bool show_moves = false);
    
    // Print current scores
    static void print_scores(int black_score, int white_score);
    
    // Print welcome message
    static void print_welcome_message();
    
    // Print game over screen
    static void print_game_over(GameStatus status, int black_score, int white_score);
    
    // Print player move notification
    static void print_player_move(PlayerColor player, int row, int col);
    
    // Print skip turn notification
    static void print_skip_turn(PlayerColor player);
    
    // Print invalid move error
    static void print_invalid_move();
    
    // Print invalid format error
    static void print_invalid_format();
};

#endif
