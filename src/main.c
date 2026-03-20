#include "game/game.h"

int main(){
    struct Game game;
    gameSetup(&game);
    gameRun(&game);
    gameShutdown(&game);
}
