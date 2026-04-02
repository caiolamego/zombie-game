#include "Game.h"

// A SDL exige que a função main seja declarada com os argumentos argc e argv [cite: 293]
int main(int argc, char** argv) {
    // A função main deve ser a mínima possível [cite: 289]
    // Deve apenas criar um Game, executá-lo, e sair do programa [cite: 290]
    
    Game& game = Game::GetInstance(); // Retornada pelo método GetInstance [cite: 291]
    game.Run();
    
    return 0;
}