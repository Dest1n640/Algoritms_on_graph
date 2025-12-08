#include "../include/GameEngine.h"
#include "../include/Player.h"
#include "../include/Display.h"

int main() {
    Display::print_welcome_message();
   
    HumanPlayer black(PlayerColor::BLACK);
    AIPlayer white(PlayerColor::WHITE, 8);
    
    GameEngine engine;
    engine.run_game(&black, &white);
    
    return 0;
}
