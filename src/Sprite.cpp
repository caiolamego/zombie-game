#include "Sprite.h"
#include "Game.h"
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"
#include <iostream>
#include "Resources.h"

Sprite::Sprite() : texture(nullptr), frameCountW(1), frameCountH(1), scale(1.0f, 1.0f), flip(SDL_FLIP_NONE) {
}

Sprite::Sprite(std::string file, int frameCountW, int frameCountH) 
    : texture(nullptr), frameCountW(frameCountW), frameCountH(frameCountH) {
    Open(file);
}

void Sprite::SetScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}
Vec2 Sprite::GetScale() const { return scale; }

void Sprite::SetFlip(bool flipH, bool flipV) {
    int f = SDL_FLIP_NONE;
    if (flipH) f |= SDL_FLIP_HORIZONTAL;
    if (flipV) f |= SDL_FLIP_VERTICAL;
    flip = (SDL_RendererFlip)f;
}

void Sprite::Open(std::string file) {
    texture = Resources::GetImage(file);
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SetFrame(0);
}

Sprite::~Sprite() {

}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void Sprite::Render(int x, int y, float angle) {
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = (int)(clipRect.w * scale.x);
    dstrect.h = (int)(clipRect.h * scale.y);
    
    // Converte ângulo de radianos para graus como a SDL pede
    double angleDeg = angle * 180.0 / M_PI;
    
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstrect, angleDeg, nullptr, flip);
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