#include "SpriteRenderer.h"
#include "GameObject.h"

// Chame o construtor de Component em sua initializer list [cite: 706]
SpriteRenderer::SpriteRenderer(GameObject& associated) : Component(associated) {}

SpriteRenderer::SpriteRenderer(GameObject& associated, std::string file, int frameCountW, int frameCountH) 
    : Component(associated), sprite(file, frameCountW, frameCountH) { // chame também o construtor de sprite [cite: 707]
    
    // Defina a altura e largura da box do GameObject associado com base no tamanho do sprite [cite: 707]
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
    SetFrame(0); // Inicialize seu primeiro frame como frame 0 [cite: 708]
}

// Wrappers diretos [cite: 711]
void SpriteRenderer::SetFrameCount(int frameCountW, int frameCountH) { sprite.SetFrameCount(frameCountW, frameCountH); }
void SpriteRenderer::SetFrame(int frame) { sprite.SetFrame(frame); }

void SpriteRenderer::Open(std::string file) {
    sprite.Open(file);
    associated.box.w = sprite.GetWidth(); // lembre de definir a altura e largura da box [cite: 712]
    associated.box.h = sprite.GetHeight();
}

void SpriteRenderer::Update(float dt) {
    // Deixe essa função vazia [cite: 714]
}

void SpriteRenderer::Render() {
    // Render do sprite usando os valores da box do GameObject [cite: 716-717]
    sprite.Render(associated.box.x, associated.box.y);
}