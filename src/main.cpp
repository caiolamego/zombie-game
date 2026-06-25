#include "Game.h"
#include <string>

int main(int argc, char** argv) {
  (void)argc; (void)argv;

  Game& game = Game::GetInstance("Caio Magalhães Lamego - 222031045", 1200, 900);
  game.Run();
  return 0;
}
