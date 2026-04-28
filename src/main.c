#include "game/game.h"
#include "stdio.h"
int main(){
    printf("Hello World\n");
    struct Game game;
    gameSetup(&game);
    gameRun(&game);
    return 0;
}
