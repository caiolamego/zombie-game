#include "State.h"
#include <SDL2/SDL.h>

State::State() : bg("recursos/img/Background.png"), music("recursos/audio/BGM.wav") { // bg aberto com img/Background.png e música com audio/BGM.wav [cite: 260, 286]
    quitRequested = false; // Inicializa quitRequested [cite: 166]
    music.Play(-1); // A música deve começar a tocar na criação do estado [cite: 287]
}

void State::LoadAssets() {
    // Por enquanto deixaremos vazio conforme as instruções de apenas carregar variáveis quando possível [cite: 168, 169]
}

void State::Update(float dt) {
    if (SDL_QuitRequested()) {
        quitRequested = true; // Se o retorno dela for true, sete a flag para true [cite: 175]
    }
}

void State::Render() {
    bg.Render(0, 0); // Chame o render do fundo (bg) passando (0,0) [cite: 178]
}

bool State::QuitRequested() {
    return quitRequested; // Retorna o valor da flag de mesmo nome [cite: 180]
}