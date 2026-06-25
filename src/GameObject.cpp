#define INCLUDE_SDL
#include "SDL_include.h"
#include "GameObject.h"
#include "Component.h"
#include <algorithm>

GameObject::GameObject()
    : box(), angleDeg(0.0), isDead(false), started(false) {}

GameObject::~GameObject() {
    for (int i = (int)components.size() - 1; i >= 0; --i) {
        delete components[i];
    }
    components.clear();
}

void GameObject::Start() {
    if (started) return;
    for (auto* c : components) c->Start();
    started = true;
}

void GameObject::Update(float dt) {
    for (auto* c : components) c->Update(dt);
}

void GameObject::Render() {
    for (auto* c : components) c->Render();
}

void GameObject::AddComponent(Component* cpt) {
    if (!cpt) return;
    components.push_back(cpt);
    if (started) {
        cpt->Start();
    }
}

void GameObject::RemoveComponent(Component* cpt) {
    auto it = std::remove(components.begin(), components.end(), cpt);
    if (it != components.end()) {
        components.erase(it, components.end());
    }
}

void GameObject::NotifyCollision(GameObject& other) {
    for (auto* c : components) {
        c->NotifyCollision(other);
    }
}
