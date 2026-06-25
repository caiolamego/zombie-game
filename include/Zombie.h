#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Component.h"
#include "Sound.h"
#include "Timer.h"

class Zombie : public Component {
public:
    explicit Zombie(GameObject& associated);
    ~Zombie();

    void Damage(int damage);

    void Start() override;
    void Update(float dt) override;
    void Render() override;

    void NotifyCollision(GameObject& other) override;   

    static int zombieCount; // contador global de zumbis vivos

private:
    int hitpoints;
    bool died;
    bool hit;
    Sound deathSound;
    Sound hitSound;
    Timer hitTimer;
    Timer deathTimer;
};

#endif
