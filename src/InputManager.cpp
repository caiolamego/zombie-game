#include "InputManager.h"

InputManager& InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager()
: keyState(), keyUpdate(),
  mouseState{false,false,false,false,false,false},
  mouseUpdate{0,0,0,0,0,0},
  quitRequested(false), updateCounter(0), mouseX(0), mouseY(0) {}

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
            case SDL_KEYDOWN: {
                if (e.key.repeat == 0) {
                    int k = e.key.keysym.sym;
                    keyState[k] = true;
                    keyUpdate[k] = updateCounter;
                }
                break;
            }
            case SDL_KEYUP: {
                int k = e.key.keysym.sym;
                keyState[k] = false;
                keyUpdate[k] = updateCounter;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                int b = e.button.button;
                if (b >= 1 && b <= 5) {
                    mouseState[b] = true; 
                    mouseUpdate[b] = updateCounter;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                int b = e.button.button;
                if (b >= 1 && b <= 5) {
                    mouseState[b] = false; 
                    mouseUpdate[b] = updateCounter;
                }
                break;
            }
        }
    }
}

bool InputManager::KeyPress(int key) {
    if (keyState.find(key) == keyState.end()) return false;
    return keyState[key] && (keyUpdate[key] == updateCounter);
}

bool InputManager::KeyRelease(int key) {
    if (keyState.find(key) == keyState.end()) return false;
    return (!keyState[key]) && (keyUpdate[key] == updateCounter);
}

bool InputManager::IsKeyDown(int key) {
    if (keyState.find(key) == keyState.end()) return false;
    return keyState[key];
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