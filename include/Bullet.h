#ifndef BULLET_H
#define BULLET_H

#include "Component.h"
#include "Vec2.h"

class Bullet : public Component {
public:
    Bullet(GameObject& associated,
           float angle,
           float speed,
           int damage,
           float maxDistance,
           bool targetsPlayer);

    void Start() override;
    void Update(float dt) override;
    void Render() override {}

    int GetDamage() const { return damage; }
    bool TargetsPlayer() const { return targetsPlayer; }

private:
    Vec2 speedVec;
    float distanceLeft;
    int damage;
    bool targetsPlayer;
};

#endif
