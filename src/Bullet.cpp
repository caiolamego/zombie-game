#include "Bullet.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Collider.h"
#include <cmath>

Bullet::Bullet(GameObject& associated,
               float angle,
               float speed,
               int dmg,
               float maxDistance,
               bool targetsPlayer)
    : Component(associated),
      speedVec(std::cos(angle) * speed, std::sin(angle) * speed),
      distanceLeft(maxDistance),
      damage(dmg),
      targetsPlayer(targetsPlayer) {

    auto* sr = new SpriteRenderer(associated, "recursos/img/Bullet.png", 1, 1);
    sr->SetScale(0.6f, 0.6f);
    associated.AddComponent(sr);

    associated.angleDeg = angle * 180.0 / M_PI;
}

void Bullet::Start() {
    // cria colisor por último para atualizar depois do movimento
    auto* col = new Collider(associated);
    col->SetScale(Vec2(0.4f, 0.4f));
    associated.AddComponent(col);
}

void Bullet::Update(float dt) {
    float dx = speedVec.x * dt;
    float dy = speedVec.y * dt;

    associated.box.x += dx;
    associated.box.y += dy;

    float d = std::sqrt(dx * dx + dy * dy);
    distanceLeft -= d;

    if (distanceLeft <= 0.0f) {
        associated.RequestDelete();
    }
}
