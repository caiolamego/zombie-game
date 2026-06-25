#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include "Game.h"
#include "State.h"
#include "Resources.h"
#include "InputManager.h"
#include "Camera.h"
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <cstdlib>


#include <stdexcept>
#include <iostream>

Game* Game::instance = nullptr;

Game& Game::GetInstance(const std::string& title, int width, int height) {
  if (!instance) {
    instance = new Game(title, width, height);
  }
  return *instance;
}

Game::Game(const std::string& title, int width, int height)
: window(nullptr), renderer(nullptr), state(nullptr), frameStart(0), dt(0.0f) {
  if (instance) {
    throw std::runtime_error("Game já inicializado (Singleton).");
  }
  instance = this;

  std::srand((unsigned)std::time(nullptr));

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
    std::cerr << "SDL_Init: " << SDL_GetError() << "\n";
    throw std::runtime_error("Falha ao inicializar SDL.");
  }

  const int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
  if ((IMG_Init(imgFlags) & imgFlags) == 0) {
    std::cerr << "IMG_Init: " << IMG_GetError() << "\n";
    throw std::runtime_error("Falha ao inicializar SDL_image.");
  }

  const int mixFlags = MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MOD;
  if ((Mix_Init(mixFlags) & mixFlags) == 0) {
    std::cerr << "Mix_Init: " << Mix_GetError() << "\n";
  }
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
    std::cerr << "Mix_OpenAudio: " << Mix_GetError() << "\n";
    throw std::runtime_error("Falha ao abrir áudio.");
  }
  Mix_AllocateChannels(32);

  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
  if (!window) {
    std::cerr << "SDL_CreateWindow: " << SDL_GetError() << "\n";
    throw std::runtime_error("Falha ao criar janela.");
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << "\n";
    throw std::runtime_error("Falha ao criar renderer.");
  }

  state = new State();

  frameStart = SDL_GetTicks();
  dt = 0.0f;
}

Game::~Game() {
  delete state;

  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();

  if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
  if (window)   { SDL_DestroyWindow(window);     window   = nullptr; }

  SDL_Quit();
}

State& Game::GetState() const {
  return *state;
}

void Game::CalculateDeltaTime() {
  unsigned int current = SDL_GetTicks();
  unsigned int diff = current - frameStart;
  dt = (float)diff / 1000.0f;
  frameStart = current;
}

void Game::Run() {
  state->Start();

  while (!state->QuitRequested()) {
    CalculateDeltaTime();
    InputManager::GetInstance().Update();

    state->Update(dt);
    state->Render();

    SDL_RenderPresent(renderer);
    SDL_Delay(33);
  }

  Resources::ClearImages();
  Resources::ClearMusics();
  Resources::ClearSounds();
}
