#ifndef GAME_H
#define GAME_H

#define INCLUDE_SDL
#include "SDL_include.h"
#include <string>

class State;

class Game {
public:
  static Game& GetInstance(const std::string& title = "IDJ",
                           int width = 1200, int height = 900);

  ~Game();

  void Run();

  SDL_Renderer* GetRenderer() const { return renderer; }
  State& GetState() const;

  void CalculateDeltaTime();
  float GetDeltaTime() const { return dt; }

private:
  Game(const std::string& title, int width, int height);
  static Game* instance;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  State* state = nullptr;

  // timing
  unsigned int frameStart = 0;
  float dt = 0.0f;
};

#endif
