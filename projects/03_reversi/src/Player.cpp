#include "../include/Player.h"
#include "../include/Display.h"
#include <limits>
#include <iostream>
#include <algorithm>
#include <thread>

namespace {
    const int EARLY_WEIGHTS[8][8] = {
        {100, -20,  10,   5,   5,  10, -20, 100},
        {-20, -50,  -5,  -5,  -5,  -5, -50, -20},
        { 10,  -5,   3,   1,   1,   3,  -5,  10},
        {  5,  -5,   1,   0,   0,   1,  -5,   5},
        {  5,  -5,   1,   0,   0,   1,  -5,   5},
        { 10,  -5,   3,   1,   1,   3,  -5,  10},
        {-20, -50,  -5,  -5,  -5,  -5, -50, -20},
        {100, -20,  10,   5,   5,  10, -20, 100}
    };
    
    const int MID_WEIGHTS[8][8] = {
        {120, -20,  20,  10,  10,  20, -20, 120},
        {-20, -40,  -5,   1,   1,  -5, -40, -20},
        { 20,  -5,  15,   3,   3,  15,  -5,  20},
        { 10,   1,   3,   2,   2,   3,   1,  10},
        { 10,   1,   3,   2,   2,   3,   1,  10},
        { 20,  -5,  15,   3,   3,  15,  -5,  20},
        {-20, -40,  -5,   1,   1,  -5, -40, -20},
        {120, -20,  20,  10,  10,  20, -20, 120}
    };
    
    const int END_WEIGHTS[8][8] = {
        {200, -10,  30,  20,  20,  30, -10, 200},
        {-10,   5,  10,  10,  10,  10,   5, -10},
        { 30,  10,  15,  10,  10,  15,  10,  30},
        { 20,  10,  10,   5,   5,  10,  10,  20},
        { 20,  10,  10,   5,   5,  10,  10,  20},
        { 30,  10,  15,  10,  10,  15,  10,  30},
        {-10,   5,  10,  10,  10,  10,   5, -10},
        {200, -10,  30,  20,  20,  30, -10, 200}
    };
}

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
    std::vector<Cell> possible_moves = engine.get_possible_moves();
    
    if (possible_moves.empty()) {
        return Cell(-1, -1, 0);
    }

    // Сортируем ходы по приоритету для улучшения альфа-бета отсечения
    std::sort(possible_moves.begin(), possible_moves.end(), 
        [this](const Cell& a, const Cell& b) {
            return get_position_priority(a.y, a.x) > get_position_priority(b.y, b.x);
        });
    
    // Определяем количество потоков (автоматически по количеству ядер процессора)
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;  // Fallback если не определилось
    
    // Ограничиваем количество потоков количеством ходов
    num_threads = std::min(num_threads, static_cast<unsigned int>(possible_moves.size()));
    
    // Массив для хранения результатов каждого потока
    std::vector<SearchResult> results(possible_moves.size());
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    
    // Запускаем потоки для параллельного поиска
    for (size_t i = 0; i < possible_moves.size(); ++i) {
        // Если достигли лимита потоков, ждем завершения предыдущих
        if (threads.size() >= num_threads) {
            threads[0].join();
            threads.erase(threads.begin());
        }
        
        // Запускаем новый поток для поиска по этому ходу
        threads.emplace_back(&AIPlayer::search_move, this, 
                            std::cref(engine), 
                            std::cref(possible_moves[i]), 
                            depth - 1, 
                            std::ref(results[i]));
    }
    
    // Ждем завершения всех оставшихся потоков
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Находим лучший результат
    SearchResult best;
    for (const auto& result : results) {
        if (result.score > best.score) {
            best = result;
        }
    }
    
    return best.move;
}

// Функция поиска для одного хода (выполняется в отдельном потоке)
void AIPlayer::search_move(const GameEngine& engine, const Cell& move, int depth, SearchResult& result) {
    GameEngine temp_engine = engine;
    temp_engine.make_move(move);
    temp_engine.switch_player();
    
    int score = minimax(temp_engine, depth, 
                       std::numeric_limits<int>::min(), 
                       std::numeric_limits<int>::max(), 
                       false);
    
    result = SearchResult(move, score);
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

    std::sort(possible_moves.begin(), possible_moves.end(),
        [this](const Cell& a, const Cell& b) {
            return get_position_priority(a.y, a.x) > get_position_priority(b.y, b.x);
        });

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
    const Board& board = engine.get_board();
    int total_pieces = engine.get_black_score() + engine.get_white_score();
    int my_color = static_cast<int>(color);
    int opp_color = -my_color;
    
    const int (*weights)[8];
    if (total_pieces < 20) {
        weights = EARLY_WEIGHTS;
    } else if (total_pieces <= 50) {
        weights = MID_WEIGHTS;
    } else {
        weights = END_WEIGHTS;
    }
    
    int score = 0;
    
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            int val = board.get_value(y, x);
            if (val == my_color) {
                score += weights[y][x];
            } else if (val == opp_color) {
                score -= weights[y][x];
            }
        }
    }
    
    if (total_pieces < 50) {
        int my_mobility = 0;
        int opp_mobility = 0;
        
        if (engine.get_current_player() == color) {
            my_mobility = engine.get_possible_moves().size();
            
            GameEngine temp_engine = engine;
            temp_engine.switch_player();
            opp_mobility = temp_engine.get_possible_moves().size();
        } else {
            opp_mobility = engine.get_possible_moves().size();
            
            GameEngine temp_engine = engine;
            temp_engine.switch_player();
            my_mobility = temp_engine.get_possible_moves().size();
        }
        
        score += (my_mobility - opp_mobility) * 30;
    }
    
    if (total_pieces > 50) {
        int piece_diff;
        if (color == PlayerColor::BLACK) {
            piece_diff = engine.get_black_score() - engine.get_white_score();
        } else {
            piece_diff = engine.get_white_score() - engine.get_black_score();
        }
        score += piece_diff * 100;
    }
    
    const int corners[4][2] = {{0,0}, {0,7}, {7,0}, {7,7}};
    int corner_bonus = 5000;
    
    for (int i = 0; i < 4; ++i) {
        int val = board.get_value(corners[i][0], corners[i][1]);
        if (val == my_color) {
            score += corner_bonus;
            score += calculate_edge_stability(board, corners[i][0], corners[i][1], my_color);
        } else if (val == opp_color) {
            score -= corner_bonus;
            score -= calculate_edge_stability(board, corners[i][0], corners[i][1], opp_color);
        }
    }
    
    return score;
}

int AIPlayer::get_position_priority(int y, int x) const {
    return EARLY_WEIGHTS[y][x];
}

int AIPlayer::calculate_edge_stability(const Board& board, int corner_y, int corner_x, int player_color) const {
    int stability = 0;
    
    int dx = (corner_x == 0) ? 1 : -1;
    int x = corner_x + dx;
    while (x >= 0 && x < 8 && board.get_value(corner_y, x) == player_color) {
        stability += 500;
        x += dx;
    }
    
    int dy = (corner_y == 0) ? 1 : -1;
    int y = corner_y + dy;
    while (y >= 0 && y < 8 && board.get_value(y, corner_x) == player_color) {
        stability += 500;
        y += dy;
    }
    
    return stability;
}
