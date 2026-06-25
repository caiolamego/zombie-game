#include "Zombie.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Animation.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Camera.h"
#include "Collider.h"
#include "Bullet.h"
#include "Character.h"  
#include "Vec2.h" 

int Zombie::zombieCount = 0;

Zombie::Zombie(GameObject& associated)
    : Component(associated),
      hitpoints(100),
      died(false),
      hit(false),
      deathSound("recursos/audio/Dead.wav"),
      hitSound("recursos/audio/Hit0.wav"),
      hitTimer(),
      deathTimer() {

    ++zombieCount;

    auto* sr = new SpriteRenderer(associated, "recursos/img/Enemy.png", 3, 2);
    sr->SetFrame(1);
    associated.AddComponent(sr);

    auto* anim = new Animator(associated);
    // andando pra direita
    anim->AddAnimation("walking_right", Animation(0, 3, 0.10f, SDL_FLIP_NONE));
    // andando pra esquerda (mesmos frames, flipados)
    anim->AddAnimation("walking_left",  Animation(0, 3, 0.10f, SDL_FLIP_HORIZONTAL));

    anim->AddAnimation("hit",  Animation(4, 4, 0.0f));
    anim->AddAnimation("dead", Animation(5, 5, 0.0f));

    anim->SetAnimation("walking_right");
    associated.AddComponent(anim);

}

Zombie::~Zombie() {
    --zombieCount;
}

void Zombie::Start() {
    auto* col = new Collider(associated);
    col->SetScale(Vec2(0.5f, 0.9f));
    col->SetOffset(Vec2(0.0f, 6.0f));
    associated.AddComponent(col);
}

void Zombie::Damage(int damage) {
    if (died) return;

    int before = hitpoints;
    hitpoints -= damage;

    if (hitSound.IsOpen()) hitSound.Play(1);

    if (before > 0 && hitpoints <= 0) {
        died = true;

        if (auto* anim = associated.GetComponent<Animator>()) {
            anim->SetAnimation("dead");
        } else if (auto* sr = associated.GetComponent<SpriteRenderer>()) {
            sr->SetFrame(5);
        }

        if (deathSound.IsOpen()) deathSound.Play(1);
        deathTimer.Restart();

        // remove colisor para não dar dano morto
        if (auto* col = associated.GetComponent<Collider>()) {
            associated.RemoveComponent(col);
          
        }
    } else {
        hit = true;
        hitTimer.Restart();
        if (auto* anim = associated.GetComponent<Animator>()) {
            anim->SetAnimation("hit");
        }
    }
}

void Zombie::Update(float dt) {
    hitTimer.Update(dt);
    deathTimer.Update(dt);

    // (click pra testar ainda funciona, se quiser)
   /*auto& im = InputManager::GetInstance();
    if (im.MousePress(LEFT_MOUSE_BUTTON)) {
        int worldX = im.GetMouseX() + (int)Camera::GetInstance().pos.x;
        int worldY = im.GetMouseY() + (int)Camera::GetInstance().pos.y;
        if (associated.box.Contains((float)worldX, (float)worldY)) {
            Damage(34);
        }
    }*/

    if (hit && !died) {
        if (hitTimer.Get() > 1.2f) {
            hit = false;
            if (auto* anim = associated.GetComponent<Animator>()) {
                anim->SetAnimation("walking");
            }
        }
    }

    if (died) {
        if (deathTimer.Get() > 5.0f) {
            associated.RequestDelete();
        }
    } else {
    if (Character::player) {
        Vec2 myCenter = associated.box.Center();
        Vec2 playerCenter = Character::player->GetGameObject().box.Center();
        Vec2 dir = (playerCenter - myCenter).Normalized();
        const float speed = 80.0f;

        associated.box.x += dir.x * speed * dt;
        associated.box.y += dir.y * speed * dt;

        if (auto* anim = associated.GetComponent<Animator>()) {
            if (dir.x < 0.0f)
                anim->SetAnimation("walking_left");
            else
                anim->SetAnimation("walking_right");
              }
        }
    }
}

void Zombie::NotifyCollision(GameObject& other) {
    if (died) return;

    if (auto* bullet = other.GetComponent<Bullet>()) {
        // só toma bala do player (targetsPlayer == false)
        if (!bullet->TargetsPlayer()) {
            Damage(bullet->GetDamage());
            other.RequestDelete();
        }
    }
}

void Zombie::Render() {
}
