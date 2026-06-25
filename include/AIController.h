#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "Component.h"
#include "Timer.h"
#include "Vec2.h"

class AIController : public Component {
public:
    explicit AIController(GameObject& associated);
    void Update(float dt) override;
    void Render() override {}

private:
    enum class AIState { MOVING, RESTING };

    int npcCounter;
    AIState state;
    Timer restTimer;
    Vec2 destination;
    float restCooldown;
};

#endif
