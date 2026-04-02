#include "Game.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

Game* Game::instance = nullptr;

Game& Game::GetInstance() {
    if (instance == nullptr) {
        // Se não existir, instancie a primeira (e única!) instância usando new [cite: 68]
        instance = new Game("Caio - Matricula: 222031045", 1200, 900); 
    }
    return *instance; // Retorna a referência [cite: 69]
}

Game::Game(std::string title, int width, int height) {
    if (instance != nullptr) {
        std::cout << "Erro lógico: Multiplas instâncias de Game." << std::endl; // Se já existir, há um problema na lógica [cite: 72]
        return;
    }
    instance = this; // Se não existir, atribua this a instance [cite: 72]

    // Inicializa a biblioteca SDL [cite: 88]
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        std::cout << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl; // Abortar com mensagem de erro se falhar [cite: 90]
    }

    // Inicialização da SDL_Image [cite: 93, 94]
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
        std::cout << "Erro ao inicializar SDL_Image: " << SDL_GetError() << std::endl;
    }

    // Inicialização da biblioteca de sons [cite: 100, 117]
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
        std::cout << "Erro ao inicializar SDL_Mixer: " << SDL_GetError() << std::endl;
    }
    Mix_AllocateChannels(32); // Vamos mudar para 32 com Mix_AllocateChannels [cite: 120]

    // Criar a janela centralizada [cite: 124, 125, 126]
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cout << "Erro ao criar janela: " << SDL_GetError() << std::endl;
    }

    // Criar o renderizador acelerado [cite: 130, 133, 137]
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Erro ao criar renderizador: " << SDL_GetError() << std::endl;
    }

    // Instanciar o estado [cite: 140]
    state = new State();
}

Game::~Game() {
    delete state; // Deleta o estado [cite: 143]
    Mix_CloseAudio(); // Encerra a SDL_Music [cite: 143]
    Mix_Quit();
    IMG_Quit(); // Encerra a SDL_Image [cite: 143]
    SDL_DestroyRenderer(renderer); // Destrói o renderizador [cite: 143]
    SDL_DestroyWindow(window); // Destrói a janela [cite: 143]
    SDL_Quit(); // Encerra a SDL [cite: 143]
}

State& Game::GetState() {
    return *state; // Retorna *state [cite: 146]
}

SDL_Renderer* Game::GetRenderer() {
    return renderer; // Retorna o membro renderer [cite: 149]
}

void Game::Run() {
    // Funciona enquanto QuitRequested não retornar true [cite: 183]
    while (!state->QuitRequested()) {
        state->Update(0); // Passe 0 como parâmetro por enquanto [cite: 184]
        state->Render(); // Render do estado [cite: 184]
        SDL_RenderPresent(renderer); // Atualizar a tela [cite: 185]
        SDL_Delay(33); // Atrasar o processamento [cite: 189, 190]
    }
}