#ifndef WAVESPAWNER_H
#define WAVESPAWNER_H

#include "Component.h"
#include "Timer.h"
#include <vector>

struct Wave {
    int zombies;
    float cooldown;
    Wave(int z = 0, float cd = 0.0f) : zombies(z), cooldown(cd) {}
};

class WaveSpawner : public Component {
public:
    explicit WaveSpawner(GameObject& associated);
    void Update(float dt) override;
    void Render() override {}

private:
    int zombieCounter;
    std::vector<Wave> waves;
    Timer zombieCooldownTimer;
    int currentWave;

    void SpawnZombie();
    void SpawnNPC();
};

#endif
