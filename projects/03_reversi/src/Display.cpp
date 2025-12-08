#include "../include/Display.h"
#include <iostream>
#include <algorithm>
#include <cctype>

bool Display::parse_move(const std::string& input, int& row, int& col) {
    if (input.length() < 2) return false;
    
    std::string trimmed = input;
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.length() != 2) return false;
    
    char letter = std::tolower(trimmed[0]);
    char digit = trimmed[1];
    
    if (letter < 'a' || letter > 'h') return false;
    if (digit < '1' || digit > '8') return false;
    
    col = letter - 'a';
    row = digit - '1';

    return true;
}

std::string Display::get_player_input() {
    std::string input;
    std::cout << "Your move: ";
    std::getline(std::cin, input);
    return input;
}

void Display::print_board(const GameEngine& engine, bool show_moves) {
    const Board& board = engine.get_board();
    std::vector<Cell> valid_moves;
    
    if (show_moves) {
        valid_moves = engine.get_possible_moves();
    }
    
    std::cout << "\n  a b c d e f g h" << std::endl;
    
    for (int i = 0; i < board.get_board_size(); i++) {
        std::cout << (i + 1) << " ";
        
        for (int j = 0; j < board.get_board_size(); j++) {
            int val = board.get_value(i, j);
            
            bool is_valid_move = false;
            for (const auto& move : valid_moves) {
                if (move.y == i && move.x == j) {
                    is_valid_move = true;
                    break;
                }
            }
            
            if (is_valid_move) std::cout << "*";
            else if (val == 0) std::cout << ".";
            else if (val == 1) std::cout << "B";
            else if (val == -1) std::cout << "W";
            
            if (j < board.get_board_size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Display::print_scores(int black_score, int white_score) {
    std::cout << "Score: B=" << black_score << " W=" << white_score << std::endl;
}

void Display::print_welcome_message() {
    std::cout << "\n=== REVERSI GAME ===" << std::endl;
    std::cout << "BLACK (B) plays first" << std::endl;
    std::cout << "Input format: letter(a-h) + number(1-8)" << std::endl;
    std::cout << "Example: d3, e4, f5" << std::endl;
    std::cout << "Valid moves marked with *\n" << std::endl;
}

void Display::print_game_over(GameStatus status, int black_score, int white_score) {
    std::cout << "\n=== GAME OVER ===" << std::endl;
    std::cout << "Final Score: B=" << black_score << " W=" << white_score << std::endl;
    
    switch (status) {
        case GameStatus::BLACK_WINS:
            std::cout << "BLACK WINS!" << std::endl;
            break;
        case GameStatus::WHITE_WINS:
            std::cout << "WHITE WINS!" << std::endl;
            break;
        case GameStatus::DRAW:
            std::cout << "IT'S A DRAW!" << std::endl;
            break;
        default:
            break;
    }
}

void Display::print_player_move(PlayerColor player, int row, int col) {
    char letter = 'a' + col;
    int number = row + 1;
    
    std::string player_name = (player == PlayerColor::BLACK) ? "Black" : "White";
    std::cout << player_name << " moves to " << letter << number << "\n" << std::endl;
}

void Display::print_skip_turn(PlayerColor player) {
    std::string player_name = (player == PlayerColor::BLACK) ? "Black" : "White";
    std::cout << player_name << " has no valid moves. Skipping turn.\n" << std::endl;
}

void Display::print_invalid_move() {
    std::cout << "Invalid move! Try again.\n" << std::endl;
}

void Display::print_invalid_format() {
    std::cout << "Invalid format! Use: letter(a-h) + number(1-8)\n" << std::endl;
}
