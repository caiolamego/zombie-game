#include "State.h"
#include <SDL2/SDL.h>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "Animator.h"
#include "TileMap.h"
#include "TileSet.h"
#include "Camera.h"
#include "InputManager.h"

State::State() : music("recursos/audio/BGM.wav") {
    quitRequested = false;
    music.Play(-1);

    // 1. Criando o Background (MUITO IMPORTANTE para o mar aparecer)
    GameObject* bgObj = new GameObject(); 
    SpriteRenderer* bgSprite = new SpriteRenderer(*bgObj, "recursos/img/Background.png"); 
    bgSprite->SetCameraFollower(true);
    bgObj->AddComponent(bgSprite); 
    AddObject(bgObj); 

    // 2. Criando o Mapa (TileMap) POR CIMA do Background
    GameObject* mapObj = new GameObject();
    mapObj->box.x = 0;
    mapObj->box.y = 0;
    TileSet* tileSet = new TileSet(64, 64, "recursos/img/Tileset.png");
    TileMap* tileMap = new TileMap(*mapObj, "recursos/map/map.txt", tileSet);
    mapObj->AddComponent(tileMap);
    AddObject(mapObj); 

    // 3. Criando o Zumbi POR CIMA do mapa
    GameObject* zombieObj = new GameObject(); 
    zombieObj->box.x = 600; 
    zombieObj->box.y = 450;
    Zombie* zombieComp = new Zombie(*zombieObj);
    zombieObj->AddComponent(zombieComp);
    AddObject(zombieObj);
}

State::~State() {
    objectArray.clear(); 
}

void State::AddObject(GameObject* go) {
    objectArray.emplace_back(go); 
}

void State::LoadAssets() {
}

void State::Update(float dt) {
    Camera::GetInstance().Update(dt); // Move a câmera

    auto& im = InputManager::GetInstance();
    
    // Sair no ESC ou no X da janela
    if (im.QuitRequested() || im.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
        return;
    }

    // Criar zumbi no Espaço na coordenada global do mapa
    if (im.KeyPress(SDLK_SPACE)) {
        int worldX = im.GetMouseX() + (int)Camera::GetInstance().pos.x;
        int worldY = im.GetMouseY() + (int)Camera::GetInstance().pos.y;
        
        GameObject* go = new GameObject();
        go->box.x = (float)worldX;
        go->box.y = (float)worldY;
        Zombie* zc = new Zombie(*go);
        go->AddComponent(zc);
        AddObject(go);
    }

    for (size_t i = 0; i < objectArray.size(); ++i) {
        objectArray[i]->Update(dt);
    }

    for (size_t i = 0; i < objectArray.size(); ) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
        } else {
            ++i;
        }
    }
}

void State::Render() {
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Render();
    }
}

bool State::QuitRequested() {
    return quitRequested;
}