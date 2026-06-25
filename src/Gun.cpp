#include "Gun.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Animation.h"
#include "GameObject.h"
#include "Camera.h"
#include "Bullet.h"
#include "Game.h"
#include "State.h"
#include "Character.h"
#include <cmath>

Gun::Gun(GameObject& associated, std::weak_ptr<GameObject> ch)
: Component(associated),
  shotSound("recursos/audio/Range.wav"),
  reloadSound("recursos/audio/PumpAction.mp3"),
  cooldownState(0), cdTimer(),
  character(ch), angle(0.0f) {

  // Gun.png é 3x2 (6 frames). Cada frame ~72x28.
  auto* sr = new SpriteRenderer(associated, "recursos/img/Gun.png", 3, 2);
  associated.AddComponent(sr);

  auto* anim = new Animator(associated);
  // Linha de cima (0..2): use 0 como "idle".
  anim->AddAnimation("idle",      Animation(0, 0, 0.0f));
  // Linha de baixo (3..5): recarregando.
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

  // ---- POSICIONAMENTO ----
  // Direção do cano a partir do ângulo atual
  Vec2 dir(std::cos(angle), std::sin(angle));

  // Ponto base: centro do corpo + pequena queda para alinhar nas mãos
  const float handDrop = 8.0f;
  Vec2 base = ch->box.Center();
  base.y += handDrop;

  // Avanço para frente do personagem (distância do cano)
  const float forward = 50.0f;

  // Centro final da arma
  Vec2 gunCenter = base + dir * forward;

  // Como a box usa topo-esquerdo, convertemos de centro -> topo-esquerdo
  associated.box.x = gunCenter.x - associated.box.w * 0.5f;
  associated.box.y = gunCenter.y - associated.box.h * 0.5f;

  // ---- COOLDOWN/ANIMAÇÃO ----
  cdTimer.Update(dt);
  switch (cooldownState) {
    case 0: break; // pronta
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

  // Rotação para o render
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

    bool targetsPlayer = false;
    if (auto chGO = character.lock()) {
        if (auto* chComp = chGO->GetComponent<Character>()) {
            // se o atirador NÃO for o player, a bala mira o player
            targetsPlayer = (chComp != Character::player);
        }
    }

    bgo->AddComponent(new Bullet(*bgo, angle, 700.0f, 34, 900.0f, targetsPlayer));
    Game::GetInstance().GetState().AddObject(bgo);

    cooldownState = 1;
    cdTimer.Restart();
}
