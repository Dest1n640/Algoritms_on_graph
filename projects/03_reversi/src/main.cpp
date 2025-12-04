#include "../include/GameEngine.h"
#include "../include/AI.h"
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

// Function to parse coordinates in format "d4", "c5", etc.
bool parse_move(const std::string& input, int& row, int& col) {
    if (input.length() < 2) return false;
    
    std::string trimmed = input;
    trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());
    
    if (trimmed.length() != 2) return false;
    
    char letter = std::tolower(trimmed[0]);
    char digit = trimmed[1];
    
    // Check if first character is a letter from a to h
    if (letter < 'a' || letter > 'h') return false;
    
    // Check if second character is a digit from 1 to 8
    if (digit < '1' || digit > '8') return false;
    
    col = letter - 'a';  // a=0, b=1, ..., h=7 (column/x)
    row = digit - '1';   // 1=0, 2=1, ..., 8=7 (row/y)
    
    return true;
}

// Function to display game status
void print_status(GameEngine& engine, bool show_moves = false) {
    if (show_moves) {
        engine.display_board_with_moves();
    } else {
        engine.display_board();
    }
    std::cout << "Score: B=" << engine.get_black_score() << " W=" << engine.get_white_score() << std::endl;
}

int main() {
    GameEngine engine;
    AI ai(Player::WHITE, 7); 

    std::cout << "\nREVERSI GAME" << std::endl;
    std::cout << "You play as BLACK (B)" << std::endl;
    std::cout << "AI plays as WHITE (W)" << std::endl;
    std::cout << "Input format: letter + number (e.g., d4, c5, f3)" << std::endl;
    std::cout << "Valid moves marked with *\n" << std::endl;

    while (engine.get_game_status() == GameStatus::PLAYING) {
        print_status(engine, true);

        if (engine.get_current_player() == Player::BLACK) {
            // Human turn
            if (!engine.has_valid_moves(Player::BLACK)) {
                std::cout << "Black has no valid moves. Skipping turn.\n" << std::endl;
                engine.switch_player();
                continue;
            }

            std::string input;
            int r, c;
            std::cout << "Your move: ";
            if (!(std::cin >> input)) {
                std::cout << "Input error. Exiting game." << std::endl;
                break; 
            }

            if (!parse_move(input, r, c)) {
                std::cout << "Invalid format! Use format: letter(a-h) + number(1-8)\n" << std::endl;
                continue;
            }

            Cell move(r, c, static_cast<int>(Player::BLACK));
            
            // make_move returns true if move was valid and executed
            if (!engine.make_move(move)) {
                std::cout << "Invalid move! Try again.\n" << std::endl;
            } else {
                char letter = 'a' + c;
                int number = r + 1;
                std::cout << "Black moves to " << letter << number << "\n" << std::endl;
                engine.switch_player();
            }

        } else {
            // AI turn
            if (!engine.has_valid_moves(Player::WHITE)) {
                std::cout << "White has no valid moves. Skipping turn.\n" << std::endl;
                engine.switch_player();
                continue;
            }

            Cell best_move = ai.get_best_move(engine);
            engine.make_move(best_move);
            char letter = 'a' + best_move.x;
            int number = best_move.y + 1;
            
            print_status(engine, false);
            std::cout << "AI (Depth 7) moves to " << letter << number << "\n" << std::endl;
            engine.switch_player();
        }
    }

    print_status(engine, false);
    GameStatus status = engine.get_game_status();
    
    std::cout << "\nGAME OVER!" << std::endl;
    if (status == GameStatus::BLACK_WON) {
        std::cout << "BLACK WINS!" << std::endl;
    } else if (status == GameStatus::WHITE_WON) {
        std::cout << "WHITE (AI) WINS!" << std::endl;
    } else {
        std::cout << "DRAW!" << std::endl;
    }
    std::cout << "Final Score: B=" << engine.get_black_score() << " W=" << engine.get_white_score() << "\n" << std::endl;

    return 0;
}
