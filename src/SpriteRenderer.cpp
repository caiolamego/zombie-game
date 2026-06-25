#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Camera.h"

SpriteRenderer::SpriteRenderer(GameObject& associated) : Component(associated) {}

SpriteRenderer::SpriteRenderer(GameObject& associated, std::string file, int frameCountW, int frameCountH) 
    : Component(associated), sprite(file, frameCountW, frameCountH) { 
    
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
    SetFrame(0);
    cameraFollower = false;
}

void SpriteRenderer::SetFrameCount(int frameCountW, int frameCountH) { sprite.SetFrameCount(frameCountW, frameCountH); }
void SpriteRenderer::SetFrame(int frame) { sprite.SetFrame(frame); }

void SpriteRenderer::Open(std::string file) {
    sprite.Open(file);
    associated.box.w = sprite.GetWidth() * sprite.GetScale().x;
    associated.box.h = sprite.GetHeight() * sprite.GetScale().y;
}

void SpriteRenderer::Update(float dt) {
}

void SpriteRenderer::Render() {
    float camX = cameraFollower ? 0.0f : Camera::GetInstance().pos.x;
    float camY = cameraFollower ? 0.0f : Camera::GetInstance().pos.y;
    // Pega o ângulo do GameObject associado
    sprite.Render(associated.box.x - camX, associated.box.y - camY, associated.angleDeg);
}

void SpriteRenderer::SetCameraFollower(bool follow) { cameraFollower = follow; }

void SpriteRenderer::SetScale(float scaleX, float scaleY) {
    // 1. Repassa a ordem para o Sprite interno
    sprite.SetScale(scaleX, scaleY);
    
    // 2. Atualiza o tamanho do GameObject para acompanhar a nova escala
    associated.box.w = sprite.GetWidth() * scaleX;
    associated.box.h = sprite.GetHeight() * scaleY;
}

Vec2 SpriteRenderer::GetScale() const {
    return sprite.GetScale();
}