#pragma once
#include <vector>
#include "Rect.h"
#include "Component.h"

class GameObject {
private:
    std::vector<Component*> components;
    bool isDead;
    bool started;

public:
    Rect box;
    double angleDeg;

    GameObject();
    ~GameObject();

    void Update(float dt);
    void Start();
    void Render();
    bool IsDead();
    void RequestDelete();
    void AddComponent(Component* cpt);
    void RemoveComponent(Component* cpt);

    #include "GetComponent.h"
};