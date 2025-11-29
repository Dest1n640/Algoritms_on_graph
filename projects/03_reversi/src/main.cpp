#include "../include/GameEngine.h"
#include "../include/AI.h"
#include <iostream>
#include <limits>
#include <vector>

void print_status(GameEngine& engine) {
    engine.display_board();
    std::cout << "Black (1): " << engine.get_black_score() << " | White (-1): " << engine.get_white_score() << std::endl;
    std::cout << "Current Player: " << (engine.get_current_player() == Player::BLACK ? "Black" : "White") << std::endl;
}

int main() {
    GameEngine engine;
    AI ai(Player::WHITE, 5); 

    std::cout << "Starting Reversi Game!" << std::endl;
    std::cout << "You are Black (1). AI is White (-1)." << std::endl;
    std::cout << "Enter moves as: row col (e.g., '3 2')" << std::endl;

    while (engine.get_game_status() == GameStatus::PLAYING) {
        print_status(engine);

        if (engine.get_current_player() == Player::BLACK) {
            // Human turn
            if (!engine.has_valid_moves(Player::BLACK)) {
                std::cout << "Black has no valid moves. Skipping turn." << std::endl;
                engine.switch_player();
                continue;
            }

            std::vector<Cell> moves = engine.get_possible_moves();
            std::cout << "Possible moves: ";
            for (const auto& m : moves) {
                std::cout << "(" << m.y << "," << m.x << ") ";
            }
            std::cout << std::endl;

            int r, c;
            std::cout << "Enter row and col: ";
            if (!(std::cin >> r >> c)) {
                std::cout << "Invalid input. Exiting." << std::endl;
                break; 
            }

            if (r < 0 || r > 7 || c < 0 || c > 7) {
                 std::cout << "Coordinates out of bounds! Try again." << std::endl;
                 continue;
            }

            Cell move(r, c, static_cast<int>(Player::BLACK));
            
            // make_move returns true if move was valid and executed
            if (!engine.make_move(move)) {
                std::cout << "Invalid move! Try again." << std::endl;
            } else {
                std::cout << "Black moved to (" << r << "," << c << ")" << std::endl;
                engine.switch_player();
            }

        } else {
            // AI turn
            std::cout << "AI is thinking..." << std::endl;
            if (!engine.has_valid_moves(Player::WHITE)) {
                std::cout << "White has no valid moves. Skipping turn." << std::endl;
                engine.switch_player();
                continue;
            }

            Cell best_move = ai.get_best_move(engine);
            engine.make_move(best_move);
            std::cout << "AI (White) moved to (" << best_move.y << "," << best_move.x << ")" << std::endl;
            engine.switch_player();
        }
        std::cout << "-----------------------------------" << std::endl;
    }

    print_status(engine);
    GameStatus status = engine.get_game_status();
    if (status == GameStatus::BLACK_WON) std::cout << "Black Wins!" << std::endl;
    else if (status == GameStatus::WHITE_WON) std::cout << "White Wins!" << std::endl;
    else std::cout << "Draw!" << std::endl;

    return 0;
}
