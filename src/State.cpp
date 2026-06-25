#define INCLUDE_SDL
#include "SDL_include.h"

#include "State.h"
#include "Game.h"
#include "SpriteRenderer.h"
#include "TileSet.h"
#include "TileMap.h"
#include "InputManager.h"
#include "Camera.h"
#include "Zombie.h"

#include "Character.h"
#include "PlayerController.h"

#include <iostream>
#include <memory>

State::State()
: started(false), objectArray(), music(), quitRequested(false) {}

State::~State() {
  objectArray.clear();
}

bool State::QuitRequested() const {
  return quitRequested;
}

std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
  if (!go) return std::weak_ptr<GameObject>();
  std::shared_ptr<GameObject> ptr(go);
  objectArray.emplace_back(ptr);
  if (started) {
    go->Start();
  }
  return std::weak_ptr<GameObject>(ptr);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) const {
  for (auto& sp : objectArray) {
    if (sp.get() == go) {
      return std::weak_ptr<GameObject>(sp);
    }
  }
  return std::weak_ptr<GameObject>();
}

void State::LoadAssets() {
  music.Open("recursos/audio/BGM.wav");
  if (music.IsOpen()) {
    music.Play(-1);
  }

  // Background
  {
    auto* bg = new GameObject();
    auto* bgSr = new SpriteRenderer(*bg, "recursos/img/Background.png", 1, 1);
    bgSr->SetCameraFollower(true);
    bg->AddComponent(bgSr);
    AddObject(bg);
  }

  // TileMap
  {
    auto* mapGO = new GameObject();
    mapGO->box.x = 0.0f; mapGO->box.y = 0.0f;
    auto* ts = new TileSet(64, 64, "recursos/img/Tileset.png");
    auto* tm = new TileMap(*mapGO, "recursos/map/map.txt", ts);
    mapGO->AddComponent(tm);
    AddObject(mapGO);
  }

  // Player
  {
    auto* playerGO = new GameObject();
    playerGO->box.x = 1280.0f;
    playerGO->box.y = 1280.0f;

    auto* chr = new Character(*playerGO, "recursos/img/Player.png");
    playerGO->AddComponent(chr);

    auto* pc = new PlayerController(*playerGO);
    playerGO->AddComponent(pc);

    Camera::GetInstance().Follow(playerGO);
    AddObject(playerGO);
  }
}

void State::Start() {
  if (started) return;
  LoadAssets();
  for (auto& sp : objectArray) {
    sp->Start();
  }
  started = true;
}

void State::Update(float dt) {
  Camera::GetInstance().Update(dt);

  auto& im = InputManager::GetInstance();
  if (im.QuitRequested() || im.KeyPress(ESCAPE_KEY)) {
    quitRequested = true;
    return;
  }

  // Spawn de Zombie com SPACE (debug)
  if (im.KeyPress(SDLK_SPACE)) {
    int worldX = im.GetMouseX() + (int)Camera::GetInstance().pos.x;
    int worldY = im.GetMouseY() + (int)Camera::GetInstance().pos.y;
    auto* go = new GameObject();
    go->box.x = (float)worldX;
    go->box.y = (float)worldY;
    auto* zc = new Zombie(*go);
    go->AddComponent(zc);
    AddObject(go);
  }

  for (size_t i = 0; i < objectArray.size(); ++i) {
    objectArray[i]->Update(dt);
  }

  for (size_t i = 0; i < objectArray.size();) {
    if (objectArray[i]->IsDead()) {
      objectArray.erase(objectArray.begin() + (long)i);
    } else {
      ++i;
    }
  }
}

void State::Render() {
  SDL_Renderer* r = Game::GetInstance().GetRenderer();
  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
  SDL_RenderClear(r);

  for (auto& obj : objectArray) obj->Render();
}
