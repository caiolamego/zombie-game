#define INCLUDE_SDL
#include "SDL_include.h"

#include "InputManager.h"
#include <iostream>

InputManager& InputManager::GetInstance() {
  static InputManager instance;
  return instance;
}

InputManager::InputManager()
: keyState(), keyUpdate(),
  mouseState{false,false,false,false,false,false},
  mouseUpdate{0,0,0,0,0,0},
  quitRequested(false), updateCounter(0),
  mouseX(0), mouseY(0) {}

InputManager::~InputManager() {}

void InputManager::Update() {
  ++updateCounter;
  quitRequested = false; 

  SDL_GetMouseState(&mouseX, &mouseY);

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        quitRequested = true;
        break;
      case SDL_KEYDOWN:
      {
        if (e.key.repeat == 0) {
          int k = e.key.keysym.sym;
          keyState[k] = true;
          keyUpdate[k] = updateCounter;
        }
        break;
      }
      case SDL_KEYUP:
      {
        int k = e.key.keysym.sym;
        keyState[k] = false;
        keyUpdate[k] = updateCounter;
        break;
      }
      case SDL_MOUSEBUTTONDOWN:
      {
        int b = e.button.button;
        if (b >= 0 && b < 6) {
          mouseState[b] = true;
          mouseUpdate[b] = updateCounter;
        } else if (b >= 1 && b <= 5) {
          if (b < 6) { mouseState[b] = true; mouseUpdate[b] = updateCounter; }
        }
        break;
      }
      case SDL_MOUSEBUTTONUP:
      {
        int b = e.button.button;
        if (b >= 0 && b < 6) {
          mouseState[b] = false;
          mouseUpdate[b] = updateCounter;
        } else if (b >= 1 && b <= 5) {
          if (b < 6) { mouseState[b] = false; mouseUpdate[b] = updateCounter; }
        }
        break;
      }
      default:
        break;
    }
  }
}

bool InputManager::KeyPress(int key) {
  auto itState = keyState.find(key);
  if (itState == keyState.end()) return false;
  if (!itState->second) return false;
  auto itUpd = keyUpdate.find(key);
  if (itUpd == keyUpdate.end()) return false;
  return itUpd->second == updateCounter;
}

bool InputManager::KeyRelease(int key) {
  auto itState = keyState.find(key);
  if (itState == keyState.end()) {
    auto itUpd = keyUpdate.find(key);
    if (itUpd == keyUpdate.end()) return false;
    return (itUpd->second == updateCounter) && (keyState[key] == false);
  }
  auto itUpd = keyUpdate.find(key);
  if (itUpd == keyUpdate.end()) return false;
  return (itUpd->second == updateCounter) && (itState->second == false);
}

bool InputManager::IsKeyDown(int key) {
  auto it = keyState.find(key);
  if (it == keyState.end()) return false;
  return it->second;
}

bool InputManager::MousePress(int button) {
  if (button < 0 || button >= 6) return false;
  return mouseState[button] && (mouseUpdate[button] == updateCounter);
}

bool InputManager::MouseRelease(int button) {
  if (button < 0 || button >= 6) return false;
  return (!mouseState[button]) && (mouseUpdate[button] == updateCounter);
}

bool InputManager::IsMouseDown(int button) {
  if (button < 0 || button >= 6) return false;
  return mouseState[button];
}
