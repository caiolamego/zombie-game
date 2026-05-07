#include "Camera.h"
#include "GameObject.h"    
#include "InputManager.h"
#include "Game.h"

Camera& Camera::GetInstance() {
    static Camera instance;
    return instance;
}

Camera::Camera() : pos(0,0), speed(0,0), focus(nullptr) {}
Camera::~Camera() {}

void Camera::Follow(GameObject* newFocus) { focus = newFocus; }
void Camera::Unfollow() { focus = nullptr; }

void Camera::Update(float dt) {
    if (focus) {
        int w=0, h=0;
        SDL_Renderer* r = Game::GetInstance().GetRenderer();
        if (r) SDL_GetRendererOutputSize(r, &w, &h);
        Vec2 target = focus->box.Center();
        pos.x = target.x - (float)w * 0.5f;
        pos.y = target.y - (float)h * 0.5f;
    } else {
        float vx = 0.0f;
        float vy = 0.0f;
        auto& im = InputManager::GetInstance();
        const float cameraSpeed = 300.0f; 

        if (im.IsKeyDown(LEFT_ARROW_KEY))  vx = -cameraSpeed;
        if (im.IsKeyDown(RIGHT_ARROW_KEY)) vx =  cameraSpeed;
        if (im.IsKeyDown(UP_ARROW_KEY))    vy = -cameraSpeed;
        if (im.IsKeyDown(DOWN_ARROW_KEY))  vy =  cameraSpeed;

        pos.x += vx * dt;
        pos.y += vy * dt;
    }
}