#pragma once
#include "Component.h"
#include "Sound.h"

class Zombie : public Component { // herda de Component [cite: 724]
private:
    int hitpoints; // [cite: 724]

public:
    Sound deathSound;
    Zombie(GameObject& associated); // [cite: 724]
    void Damage(int damage); // [cite: 724]

    void Update(float dt) override; // [cite: 724]
    void Render() override; // [cite: 724]
};