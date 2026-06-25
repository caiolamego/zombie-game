#include "PlayerController.h"
#include "InputManager.h"
#include "Camera.h"
#include "Character.h"
#include "GameObject.h"

PlayerController::PlayerController(GameObject& associated) : Component(associated) {}

void PlayerController::Update(float dt) {
    auto* chr = associated.GetComponent<Character>();
    if (!chr) return;
    
    auto& im = InputManager::GetInstance();
    float dx = 0.0f, dy = 0.0f;
    
    if (im.IsKeyDown(SDLK_w)) dy -= 1.0f;
    if (im.IsKeyDown(SDLK_s)) dy += 1.0f;
    if (im.IsKeyDown(SDLK_a)) dx -= 1.0f;
    if (im.IsKeyDown(SDLK_d)) dx += 1.0f;
    
    chr->Issue(Character::Command(Character::CommandType::MOVE, dx, dy));
    
    if (im.MousePress(LEFT_MOUSE_BUTTON)) {
        int worldX = im.GetMouseX() + (int)Camera::GetInstance().pos.x;
        int worldY = im.GetMouseY() + (int)Camera::GetInstance().pos.y;
        chr->Issue(Character::Command(Character::CommandType::SHOOT, (float)worldX, (float)worldY));
    }
}