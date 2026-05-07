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
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::Update(float dt) {
}

void SpriteRenderer::Render() {
    float camX = cameraFollower ? 0 : Camera::GetInstance().pos.x;
    float camY = cameraFollower ? 0 : Camera::GetInstance().pos.y;
    
    sprite.Render(associated.box.x - camX, associated.box.y - camY);
}

void SpriteRenderer::SetCameraFollower(bool follow) { cameraFollower = follow; }