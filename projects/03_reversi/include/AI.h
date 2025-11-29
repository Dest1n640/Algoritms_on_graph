#ifndef AI_H
#define AI_H

#include "GameEngine.h"
#include <limits>

class AI {
public:
    AI(Player ai_player, int depth);
    Cell get_best_move(GameEngine& engine);

private:
    Player ai_player;
    int max_depth;

    int minimax(GameEngine engine, int depth, int alpha, int beta, bool maximizingPlayer);
    int evaluate(const GameEngine& engine);
};

#endif
