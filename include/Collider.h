#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "Rect.h"
#include "Vec2.h"

class Collider : public Component {
public:
    Collider(GameObject& associated, Vec2 scale = Vec2(1.0f, 1.0f), Vec2 offset = Vec2(0.0f, 0.0f));

    void Update(float dt) override;
    void Render() override;

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

    Rect box;

private:
    Vec2 scale;
    Vec2 offset;
};

#endif
