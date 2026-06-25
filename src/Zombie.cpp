#include "Zombie.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Animation.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Camera.h"

Zombie::Zombie(GameObject& associated)
: Component(associated), hitpoints(100), died(false), hit(false),
  deathSound("recursos/audio/Dead.wav"), hitSound("recursos/audio/Hit0.wav"),
  hitTimer(), deathTimer() {
  auto* sr = new SpriteRenderer(associated, "recursos/img/Enemy.png", 3, 2);
  sr->SetFrame(1);
  associated.AddComponent(sr);

  auto* anim = new Animator(associated);
  anim->AddAnimation("walking", Animation(0, 3, 0.10f));
  anim->AddAnimation("hit",     Animation(4, 4, 0.0f));
  anim->AddAnimation("dead",    Animation(5, 5, 0.0f));
  anim->SetAnimation("walking");
  associated.AddComponent(anim);
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

  auto& im = InputManager::GetInstance();
  if (im.MousePress(LEFT_MOUSE_BUTTON)) {
    int worldX = im.GetMouseX() + (int)Camera::GetInstance().pos.x;
    int worldY = im.GetMouseY() + (int)Camera::GetInstance().pos.y;
    if (associated.box.Contains((float)worldX, (float)worldY)) {
      Damage(34);
    }
  }

  if (hit && !died) {
    if (hitTimer.Get() > 0.5f) {
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
  }
}
void Zombie::Render() { }
