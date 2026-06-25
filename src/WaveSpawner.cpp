#include "WaveSpawner.h"
#include "Game.h"
#include "State.h"
#include "GameObject.h"
#include "Zombie.h"
#include "Character.h"
#include "AIController.h"
#include "Vec2.h"
#include <cstdlib>
#include <cmath>

WaveSpawner::WaveSpawner(GameObject& associated)
    : Component(associated),
      zombieCounter(0),
      waves(),
      zombieCooldownTimer(),
      currentWave(0) {

    // Algumas waves simples 
    waves.emplace_back(5, 1.2f);
    waves.emplace_back(8, 0.9f);
    waves.emplace_back(12, 0.7f);

    zombieCooldownTimer.Restart();

    // Cria pelo menos um NPC para competir com o jogador
    SpawnNPC();
}

void WaveSpawner::SpawnZombie() {
    if (!Character::player) return;

    GameObject& playerGO = Character::player->GetGameObject();
    Vec2 center = playerGO.box.Center();

    float angle = (float)rand() / (float)RAND_MAX * 2.0f * (float)M_PI;
    float distance = 700.0f;

    Vec2 pos = center + Vec2(std::cos(angle), std::sin(angle)) * distance;

    auto* go = new GameObject();
    go->box.x = pos.x;
    go->box.y = pos.y;

    auto* zc = new Zombie(*go);
    go->AddComponent(zc);

    Game::GetInstance().GetState().AddObject(go);
}

void WaveSpawner::SpawnNPC() {
    auto* go = new GameObject();

    if (Character::player) {
        GameObject& playerGO = Character::player->GetGameObject();
        Vec2 center = playerGO.box.Center();
        float angle = (float)rand() / (float)RAND_MAX * 2.0f * (float)M_PI;
        float distance = 400.0f;
        Vec2 pos = center + Vec2(std::cos(angle), std::sin(angle)) * distance;
        go->box.x = pos.x;
        go->box.y = pos.y;
    } else {
        go->box.x = 1000.0f;
        go->box.y = 1000.0f;
    }

    auto* chr = new Character(*go, "recursos/img/NPC.png");
    go->AddComponent(chr);

    go->AddComponent(new AIController(*go));

    Game::GetInstance().GetState().AddObject(go);
}

void WaveSpawner::Update(float dt) {
    zombieCooldownTimer.Update(dt);

    if (currentWave >= (int)waves.size()) {
        associated.RequestDelete();
        return;
    }

    Wave& w = waves[currentWave];

    // Se já spawnamos todos desta wave, espera matar todos para trocar
    if (zombieCounter >= w.zombies) {
        if (Zombie::zombieCount <= 0) {
            ++currentWave;
            zombieCounter = 0;
            zombieCooldownTimer.Restart();

            if (currentWave < (int)waves.size()) {
                SpawnNPC(); // novo NPC em cada wave
            } else {
                associated.RequestDelete();
            }
        }
        return;
    }

    // Ainda faltam zumbis: spawna respeitando cooldown
    if (zombieCooldownTimer.Get() >= w.cooldown) {
        SpawnZombie();
        ++zombieCounter;
        zombieCooldownTimer.Restart();
    }
}
