#define INCLUDE_SDL
#include "SDL_include.h"

#include "Camera.h"
#include "GameObject.h"    
#include "InputManager.h"
#include "Game.h"
#include <iostream>

Camera& Camera::GetInstance() {
  static Camera instance;
  return instance;
}

Camera::Camera() : pos(0,0), speed(0,0), focus(nullptr) {}

Camera::~Camera() {}

void Camera::Follow(GameObject* newFocus) {
  focus = newFocus;
}

void Camera::Unfollow() {
  focus = nullptr;
}

void Camera::Update(float dt) {
  if (focus) {
    // centraliza o foco na tela
    int w=0, h=0;
    SDL_Renderer* r = Game::GetInstance().GetRenderer();
    if (r) SDL_GetRendererOutputSize(r, &w, &h);
    Vec2 target = focus->box.Center();
    pos.x = target.x - (float)w * 0.5f;
    pos.y = target.y - (float)h * 0.5f;
  } else {
    // movimento manual da câmera via setas
    float vx = 0.0f;
    float vy = 0.0f;
    auto& im = InputManager::GetInstance();
    const float cameraSpeed = 300.0f; 

    if (im.IsKeyDown(SDLK_LEFT))  vx = -cameraSpeed;
    if (im.IsKeyDown(SDLK_RIGHT)) vx =  cameraSpeed;
    if (im.IsKeyDown(SDLK_UP))    vy = -cameraSpeed;
    if (im.IsKeyDown(SDLK_DOWN))  vy =  cameraSpeed;

    pos.x += vx * dt;
    pos.y += vy * dt;
    
  }
}
