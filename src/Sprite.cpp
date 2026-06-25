#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

#include "Sprite.h"
#include "Game.h"
#include "Resources.h"
#include <iostream>
#include <cmath>

Sprite::Sprite()
: texture(nullptr),
  sheetW(0), sheetH(0),
  frameW(0), frameH(0),
  framesW(1), framesH(1),
  clipRect{0,0,0,0},
  flip(SDL_FLIP_NONE),
  scale(1.0f, 1.0f) {}

Sprite::Sprite(const std::string& file, int frameCountW, int frameCountH)
: Sprite() {
  framesW = (frameCountW > 0) ? frameCountW : 1;
  framesH = (frameCountH > 0) ? frameCountH : 1;
  Open(file);
}

Sprite::~Sprite() {
  texture = nullptr;
}

void Sprite::RecalcFrameSize() {
  if (framesW <= 0) framesW = 1;
  if (framesH <= 0) framesH = 1;
  frameW = (framesW > 0) ? (sheetW / framesW) : sheetW;
  frameH = (framesH > 0) ? (sheetH / framesH) : sheetH;
  SetClip(0, 0, frameW, frameH);
}

void Sprite::Open(const std::string& file) {
  texture = Resources::GetImage(file);
  if (!texture) {
    std::cerr << "IMG Load (Resources) falhou (arquivo: " << file << ")\n";
    sheetW = sheetH = frameW = frameH = 0;
    clipRect = {0,0,0,0};
    return;
  }

  if (SDL_QueryTexture(texture, nullptr, nullptr, &sheetW, &sheetH) != 0) {
    std::cerr << "SDL_QueryTexture: " << SDL_GetError() << "\n";
  }

  RecalcFrameSize();
}

void Sprite::SetClip(int x, int y, int w, int h) {
  clipRect.x = x;
  clipRect.y = y;
  clipRect.w = w;
  clipRect.h = h;
}

void Sprite::Render(int x, int y) {
  if (!texture) return;
  SDL_Rect dst{ x, y, GetWidth(), GetHeight() };
  SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dst, 0.0, nullptr, flip);
}

void Sprite::Render(int x, int y, int w, int h) {
  if (!texture) return;
  SDL_Rect dst{ x, y, w, h };
  SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dst, 0.0, nullptr, flip);
}

void Sprite::RenderEx(int x, int y, int w, int h, double angleDeg) {
  if (!texture) return;
  SDL_Rect dst{ x, y, w, h };
  SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dst, angleDeg, nullptr, flip);
}

void Sprite::SetFrame(int frame) {
  if (!texture) return;
  if (frame < 0) frame = 0;

  int totalFrames = framesW * framesH;
  if (frame >= totalFrames) frame = totalFrames - 1;

  int row = frame / framesW;
  int col = frame % framesW;

  int cx = col * frameW;
  int cy = row * frameH;

  if (cx + frameW > sheetW) cx = sheetW - frameW;
  if (cy + frameH > sheetH) cy = sheetH - frameH;

  SetClip(cx, cy, frameW, frameH);
}

void Sprite::SetFrameCount(int frameCountW, int frameCountH) {
  framesW = (frameCountW > 0) ? frameCountW : 1;
  framesH = (frameCountH > 0) ? frameCountH : 1;

  RecalcFrameSize();
  SetFrame(0);
}

void Sprite::SetScale(float sx, float sy) {
  if (sx != 0.0f) scale.x = sx;
  if (sy != 0.0f) scale.y = sy;
}

void Sprite::SetFlip(SDL_RendererFlip f) {
  flip = f;
}
