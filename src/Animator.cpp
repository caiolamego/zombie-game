#include "Animator.h"
#include "GameObject.h"
#include "SpriteRenderer.h"

// Inicialize variáveis como 0 [cite: 767]
Animator::Animator(GameObject& associated) : Component(associated), frameStart(0), frameEnd(0), frameTime(0), currentFrame(0), timeElapsed(0) {}

void Animator::AddAnimation(std::string name, Animation anim) {
    // Se não encontrar, insira o par no mapa [cite: 780-781]
    if (animations.find(name) == animations.end()) {
        animations[name] = anim;
    }
}

void Animator::SetAnimation(std::string name) {
    // Busque no map [cite: 776]
    auto it = animations.find(name);
    if (it != animations.end()) { // Se encontrar [cite: 776-777]
        frameStart = it->second.frameStart;
        frameEnd = it->second.frameEnd; // defina frameStart, frameEnd e frameTime [cite: 777]
        frameTime = it->second.frameTime;
        
        currentFrame = frameStart; // Resete currentFrame para ser igual a frameStart [cite: 778]
        timeElapsed = 0; // e timeElapsed para 0 [cite: 778]
        
        SpriteRenderer* spriteRend = associated.GetComponent<SpriteRenderer>(); // use GetComponent() [cite: 770]
        if (spriteRend) spriteRend->SetFrame(currentFrame); // chame SetFrame no SpriteRenderer [cite: 778-779]
    }
}

void Animator::Update(float dt) {
    if (frameTime == 0) return; // Se frameTime for 0, não há animação [cite: 768]

    timeElapsed += 1; // Para esse trabalho, usamos += 1 porque ainda não usamos o dt real [cite: 769, 771]
    
    if (timeElapsed > frameTime) { // Se timeElapsed for maior que frameTime [cite: 769]
        currentFrame++; // incremente currentFrame [cite: 769]
        timeElapsed -= frameTime; // subtraia frameTime [cite: 769]
        
        if (currentFrame > frameEnd) { // Se currentFrame for maior que frameEnd [cite: 769-770]
            currentFrame = frameStart; // volte para frameStart [cite: 770]
        }
        
        SpriteRenderer* spriteRend = associated.GetComponent<SpriteRenderer>(); // use GetComponent() [cite: 770]
        if (spriteRend) spriteRend->SetFrame(currentFrame); // chame SetFrame [cite: 770]
    }
}

void Animator::Render() {
    // Deixe vazia [cite: 774]
}