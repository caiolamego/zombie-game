#include "Zombie.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"

Zombie::Zombie(GameObject& associated) : Component(associated), hitpoints(100), deathSound("recursos/audio/Dead.wav") {
    SpriteRenderer* sprite = new SpriteRenderer(associated, "recursos/img/Enemy.png", 3, 2); 
    associated.AddComponent(sprite);

    Animator* animator = new Animator(associated);
    animator->AddAnimation("walking", Animation(0, 3, 10));
    animator->AddAnimation("dead", Animation(5, 5, 0));
    associated.AddComponent(animator);
    
    animator->SetAnimation("walking");
}

void Zombie::Damage(int damage) {
    if (hitpoints > 0) { // Garante que não é um zumbi morto tomando dano extra
        hitpoints -= damage;
        if (hitpoints <= 0) {
            Animator* anim = associated.GetComponent<Animator>();
            if (anim) anim->SetAnimation("dead");
            deathSound.Play(1); // Toca o som só no exato momento da morte!
        }
    }
}

void Zombie::Update(float dt) {
    Damage(1);
}

void Zombie::Render() {

}