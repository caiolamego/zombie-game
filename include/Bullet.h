#pragma once
#include "Component.h"
#include "Vec2.h"

class Bullet : public Component {
public:
    Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance);
    
    void Update(float dt) override;
    void Render() override;
    int GetDamage() const { return damage; }

private:
    Vec2 speedVec;
    float distanceLeft;
    int damage;
};