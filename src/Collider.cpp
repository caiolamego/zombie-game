#define INCLUDE_SDL
#include "SDL_include.h"
#include "Collider.h"
#include "GameObject.h"
#include "Camera.h"
#include "Game.h"
#include <cmath>

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset)
    : Component(associated), box(), scale(scale), offset(offset) {
}

void Collider::Update(float) {
    Rect base = associated.box;

    box.w = base.w * scale.x;
    box.h = base.h * scale.y;

    Vec2 center = base.Center();

    float angleRad = (float)(associated.angleDeg * M_PI / 180.0);
    Vec2 offRot = offset.Rotated(angleRad);

    Vec2 newCenter = center + offRot;
    box.x = newCenter.x - box.w * 0.5f;
    box.y = newCenter.y - box.h * 0.5f;
}

void Collider::Render() {

}

void Collider::SetScale(Vec2 s) {
    scale = s;
}

void Collider::SetOffset(Vec2 o) {
    offset = o;
}
