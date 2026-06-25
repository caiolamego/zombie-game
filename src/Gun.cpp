#include "Gun.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Animation.h"
#include "GameObject.h"
#include "Camera.h"
#include "Bullet.h"
#include "Game.h"
#include "State.h"
#include <cmath>

Gun::Gun(GameObject& associated, std::weak_ptr<GameObject> ch) : Component(associated),
  shotSound("recursos/audio/Range.wav"),
  reloadSound("recursos/audio/PumpAction.mp3"),
  cooldownState(0), cdTimer(),
  character(ch), angle(0.0f) {
      
  auto* sr = new SpriteRenderer(associated, "recursos/img/Gun.png", 3, 2);
  associated.AddComponent(sr);
  
  auto* anim = new Animator(associated);
  anim->AddAnimation("idle",      Animation(0, 0, 0.0f));
  anim->AddAnimation("reloading", Animation(3, 5, 0.10f));
  anim->SetAnimation("idle");
  associated.AddComponent(anim);
}

void Gun::Update(float dt) {
  auto ch = character.lock();
  if (!ch) {
    associated.RequestDelete();
    return;
  }
  
  Vec2 dir(std::cos(angle), std::sin(angle));
  const float handDrop = 8.0f;
  Vec2 base = ch->box.Center();
  base.y += handDrop;
  
  const float forward = 50.0f;
  Vec2 gunCenter = base + dir * forward;
  
  associated.box.x = gunCenter.x - associated.box.w * 0.5f;
  associated.box.y = gunCenter.y - associated.box.h * 0.5f;
  
  cdTimer.Update(dt);
  switch (cooldownState) {
    case 0: break;
    case 1:
      if (cdTimer.Get() > 0.12f) {
        cooldownState = 2;
        cdTimer.Restart();
        if (reloadSound.IsOpen()) reloadSound.Play(1);
        if (auto* a = associated.GetComponent<Animator>()) a->SetAnimation("reloading");
      }
      break;
    case 2:
      if (cdTimer.Get() > 0.35f) {
        cooldownState = 3;
        cdTimer.Restart();
        if (auto* a = associated.GetComponent<Animator>()) a->SetAnimation("idle");
      }
      break;
    case 3:
      if (cdTimer.Get() > 0.10f) {
        cooldownState = 0;
        cdTimer.Restart();
      }
      break;
  }
  associated.angleDeg = angle * 180.0 / M_PI;
}

void Gun::Shoot(Vec2 target) {
  if (cooldownState != 0) return;
  Vec2 center = associated.box.Center();
  Vec2 to = target - center;
  angle = std::atan2(to.y, to.x);
  
  if (shotSound.IsOpen()) shotSound.Play(1);
  
  const float muzzleOff = 28.0f;
  Vec2 dir(std::cos(angle), std::sin(angle));
  Vec2 spawn = center + dir * muzzleOff;
  
  auto* bgo = new GameObject();
  bgo->box.x = spawn.x - 8.0f;
  bgo->box.y = spawn.y - 8.0f;
  bgo->AddComponent(new Bullet(*bgo, angle, 700.0f, 34, 900.0f));
  Game::GetInstance().GetState().AddObject(bgo);
  
  cooldownState = 1;
  cdTimer.Restart();
}

void Gun::Render() {}