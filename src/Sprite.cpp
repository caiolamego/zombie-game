#include "Sprite.h"
#include "Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Sprite::Sprite() {
    texture = nullptr; // Seta texture como nullptr [cite: 217]
}

Sprite::Sprite(std::string file) {
    texture = nullptr;
    Open(file); // Em seguida, chama Open [cite: 219]
}

Sprite::~Sprite() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture); // Desaloca a textura [cite: 221]
    }
}

void Sprite::Open(std::string file) {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture); // Se já houver imagem carregada, deve ser desalocada primeiro [cite: 223]
    }
    
    // Carrega a textura usando o renderizador de Game [cite: 224, 226]
    texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (texture == nullptr) {
        std::cout << "Erro ao carregar textura: " << SDL_GetError() << std::endl;
    }
    
    // Obtém as dimensões da textura [cite: 230, 233]
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    
    // Seta o clip com as dimensões da imagem [cite: 234]
    SetClip(0, 0, width, height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h; // Seta clipRect com os parâmetros dados [cite: 236]
}

void Sprite::Render(int x, int y) {
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = clipRect.w;
    dstrect.h = clipRect.h; // weh assumem os valores contidos no clipRect [cite: 255]
    
    // Wrapper para SDL_RenderCopy [cite: 238]
    SDL_RenderCopy(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstrect);
}

int Sprite::GetWidth() {
    return width; // Retorna as dimensões da imagem [cite: 257]
}

int Sprite::GetHeight() {
    return height; // Retorna as dimensões da imagem [cite: 257]
}

bool Sprite::IsOpen() {
    return texture != nullptr; // Retorna true se texture estiver alocada [cite: 259]
}