#include "Game.h"
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include <iostream>
#include "Resources.h"
#include "InputManager.h"

Game* Game::instance = nullptr;

Game& Game::GetInstance() {
    if (instance == nullptr) {
        instance = new Game("Caio - Matricula: 222031045", 1200, 900); 
    }
    return *instance;
}

Game::Game(std::string title, int width, int height) {
    if (instance != nullptr) {
        std::cout << "Erro lógico: Multiplas instâncias de Game." << std::endl;
        return;
    }
    instance = this;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        std::cout << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
        std::cout << "Erro ao inicializar SDL_Image: " << SDL_GetError() << std::endl;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
        std::cout << "Erro ao inicializar SDL_Mixer: " << SDL_GetError() << std::endl;
    }
    Mix_AllocateChannels(32);

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cout << "Erro ao criar janela: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Erro ao criar renderizador: " << SDL_GetError() << std::endl;
    }

    state = new State();
}

Game::~Game() {
    delete state;
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit(); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window); 
    SDL_Quit();
}

State& Game::GetState() {
    return *state;
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

void Game::Run() {
    state->LoadAssets();

    while (!state->QuitRequested()) {
        CalculateDeltaTime(); // Calcula o tempo que passou
        
        InputManager::GetInstance().Update(); // Atualiza teclado e mouse
        
        state->Update(dt); // Agora o update recebe o tempo real
        
        SDL_RenderClear(renderer);
        state->Render();
        SDL_RenderPresent(renderer);
        
        SDL_Delay(33);
    }
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
}


void Game::CalculateDeltaTime() {
    unsigned int current = SDL_GetTicks();
    unsigned int diff = current - frameStart;
    dt = (float)diff / 1000.0f;
    frameStart = current;
}