#include "Sprite.h"
#include "Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Sprite::Sprite() : texture(nullptr), frameCountW(1), frameCountH(1) {
}

Sprite::Sprite(std::string file, int frameCountW, int frameCountH) 
    : texture(nullptr), frameCountW(frameCountW), frameCountH(frameCountH) {
    Open(file);
}

Sprite::~Sprite() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Sprite::Open(std::string file) {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    
    texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (texture == nullptr) {
        std::cout << "Erro ao carregar textura: " << SDL_GetError() << std::endl;
    }
    
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    SetFrame(0);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void Sprite::Render(int x, int y) {
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = clipRect.w;
    dstrect.h = clipRect.h;
    
    SDL_RenderCopy(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstrect);
}

void Sprite::SetFrameCount(int frameCountW, int frameCountH) {
    this->frameCountW = frameCountW;
    this->frameCountH = frameCountH;
}

void Sprite::SetFrame(int frame) {
    int currentColumn = frame % frameCountW;
    int currentRow = frame / frameCountW;
    int frameWidth = GetWidth();
    int frameHeight = GetHeight();

    SetClip(currentColumn * frameWidth, currentRow * frameHeight, frameWidth, frameHeight);
}

int Sprite::GetWidth() {
    return width / frameCountW; 
}

int Sprite::GetHeight() {
    return height / frameCountH;
}

bool Sprite::IsOpen() {
    return texture != nullptr;
}