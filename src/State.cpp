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
#include <algorithm> // Necessário para o Y-Sorting

State::State() : objectArray(), music(), quitRequested(false), started(false) {}

State::~State() {
    objectArray.clear(); 
}

bool State::QuitRequested() const {
    return quitRequested;
}

// Retorna weak_ptr e adiciona ao vetor como shared_ptr
std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
    if (!go) return std::weak_ptr<GameObject>();
    
    std::shared_ptr<GameObject> ptr(go);
    objectArray.push_back(ptr);
    
    // Se o state já iniciou, roda o start do objeto imediatamente
    if (started) {
        ptr->Start();
    }
    return std::weak_ptr<GameObject>(ptr);
}

// Busca o shared_ptr no array e retorna como weak_ptr
std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
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

    // 1. Criando o Background
    auto* bgObj = new GameObject(); 
    auto* bgSprite = new SpriteRenderer(*bgObj, "recursos/img/Background.png"); 
    bgSprite->SetCameraFollower(true);
    bgObj->AddComponent(bgSprite); 
    AddObject(bgObj); 

    // 2. Criando o Mapa (TileMap)
    auto* mapObj = new GameObject();
    mapObj->box.x = 0.0f;
    mapObj->box.y = 0.0f;
    auto* ts = new TileSet(64, 64, "recursos/img/Tileset.png");
    auto* tm = new TileMap(*mapObj, "recursos/map/map.txt", ts);
    mapObj->AddComponent(tm);
    AddObject(mapObj); 

    // 3. Criando o Jogador no centro do mapa
    auto* playerGO = new GameObject();
    playerGO->box.x = 1280.0f;
    playerGO->box.y = 1280.0f;
    auto* chr = new Character(*playerGO, "recursos/img/Player.png");
    playerGO->AddComponent(chr);
    auto* pc = new PlayerController(*playerGO);
    playerGO->AddComponent(pc);
    Camera::GetInstance().Follow(playerGO); // A câmera agora segue o jogador
    AddObject(playerGO);
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
    Camera::GetInstance().Update(dt); // Move a câmera

    auto& im = InputManager::GetInstance();
    
    // Sair no ESC ou no X da janela
    if (im.QuitRequested() || im.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
        return;
    }

    // Criar zumbi no clique do espaço (para testes)
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

    // Limpeza de memória inteligente
    for (size_t i = 0; i < objectArray.size(); ) {
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

    // Bônus: Y-Sorting. Ordena o array na hora de desenhar baseado na posição Y
    std::vector<std::shared_ptr<GameObject>> sortedArray = objectArray;
    std::sort(sortedArray.begin(), sortedArray.end(), [](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) {
        return (a->box.y + a->box.h) < (b->box.y + b->box.h);
    });

    for (auto& obj : sortedArray) {
        obj->Render();
    }
}