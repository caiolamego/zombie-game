#include "AIController.h"
#include "GameObject.h"
#include "Character.h"
#include <cmath>

AIController::AIController(GameObject& associated)
    : Component(associated),
      npcCounter(0),
      state(AIState::RESTING),
      restTimer(),
      destination(),
      restCooldown(1.5f) {
}

void AIController::Update(float dt) {
    auto* chr = associated.GetComponent<Character>();
    if (!chr) return;
    if (chr->IsDead()) return; 
    if (!Character::player) return;

    restTimer.Update(dt);

    Vec2 myCenter = associated.box.Center();
    Vec2 playerCenter = Character::player->GetGameObject().box.Center();

    switch (state) {
    case AIState::RESTING:
        if (restTimer.Get() >= restCooldown) {
            state = AIState::MOVING;
            destination = playerCenter;
            restTimer.Restart();
        }
        break;

    case AIState::MOVING: {
        Vec2 toDest = destination - myCenter;
        float dist = toDest.Magnitude();
        const float distThresh = 20.0f;

        if (dist > distThresh) {
            Vec2 dir = toDest.Normalized();
            // usa o sistema de comandos do Character
            chr->Issue(Character::Command(Character::CommandType::MOVE, dir.x, dir.y));
        } else {
            // Atira na posição atual do player
            chr->Issue(Character::Command(Character::CommandType::SHOOT, playerCenter.x, playerCenter.y));
            state = AIState::RESTING;
            restTimer.Restart();
        }
        break;
    }
    }
}
