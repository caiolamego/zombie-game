#define INCLUDE_SDL
#include "SDL_include.h"

#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Camera.h"

SpriteRenderer::SpriteRenderer(GameObject& associated)
: Component(associated), sprite(), cameraFollower(false) {}

SpriteRenderer::SpriteRenderer(GameObject& associated, const std::string& file, int fw, int fh)
: Component(associated), sprite(file, fw, fh), cameraFollower(false) {
  associated.box.w = (float)sprite.GetWidth();
  associated.box.h = (float)sprite.GetHeight();
  SetFrame(0);
}

void SpriteRenderer::Open(const std::string& file) {
  sprite.Open(file);
  associated.box.w = (float)sprite.GetWidth();
  associated.box.h = (float)sprite.GetHeight();
}

void SpriteRenderer::SetFrameCount(int fw, int fh) {
  sprite.SetFrameCount(fw, fh);
  associated.box.w = (float)sprite.GetWidth();
  associated.box.h = (float)sprite.GetHeight();
}

void SpriteRenderer::SetFrame(int frame) {
  sprite.SetFrame(frame);
}

void SpriteRenderer::SetFrame(int frame, SDL_RendererFlip flip) {
  sprite.SetFlip(flip);
  sprite.SetFrame(frame);
}

void SpriteRenderer::SetScale(float sx, float sy) {
  auto c = associated.box.Center();
  sprite.SetScale(sx, sy);
  associated.box.w = (float)sprite.GetWidth();
  associated.box.h = (float)sprite.GetHeight();
  auto nc = associated.box.Center();
  auto dx = c.x - nc.x;
  auto dy = c.y - nc.y;
  associated.box.x += dx;
  associated.box.y += dy;
}

void SpriteRenderer::Update(float) { }

void SpriteRenderer::Render() {
  int x = (int)associated.box.x;
  int y = (int)associated.box.y;
  if (!cameraFollower) {
    x -= (int)Camera::GetInstance().pos.x;
    y -= (int)Camera::GetInstance().pos.y;
  }
  sprite.RenderEx(x, y, (int)associated.box.w, (int)associated.box.h, associated.angleDeg);
}
