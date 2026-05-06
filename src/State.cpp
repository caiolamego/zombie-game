#include "State.h"
#define INCLUDE_SDL
#include "SDL_include.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Zombie.h"
#include "Animator.h"
#include "TileSet.h"
#include "TileMap.h"

// O construtor inicializa a música e cria nossos primeiros objetos [cite: 718-721, 738-739]
State::State() : music("recursos/audio/BGM.wav") {
    quitRequested = false;
    music.Play(-1);

    // 1. Criando o Mapa (TileSet e TileMap)
    GameObject* mapObj = new GameObject();
    mapObj->box.x = 0;
    mapObj->box.y = 0;
    TileSet* tileSet = new TileSet(64, 64, "recursos/img/Tileset.png");
    TileMap* tileMap = new TileMap(*mapObj, "recursos/map/map.txt", tileSet);
    mapObj->AddComponent(tileMap);
    AddObject(mapObj); // Adicionado primeiro para desenhar o fundo antes dos personagens

    // 2. Criando o Zumbi (Pode criar mais de um agora que temos Cache de texturas!)
    GameObject* zombieObj = new GameObject(); 
    zombieObj->box.x = 600; 
    zombieObj->box.y = 450;
    Zombie* zombieComp = new Zombie(*zombieObj);
    zombieObj->AddComponent(zombieComp);
    AddObject(zombieObj);
}

// Destrutor limpa o array de ponteiros inteligentes [cite: 611]
State::~State() {
    objectArray.clear(); 
}

// Adiciona um novo GameObject ao final do vetor [cite: 622]
void State::AddObject(GameObject* go) {
    objectArray.emplace_back(go); 
}

// Método vazio por enquanto
void State::LoadAssets() {
}

// Atualiza a lógica do estado
void State::Update(float dt) {
    if (SDL_QuitRequested()) {
        quitRequested = true;
    }

    // Atualiza todos os objetos da cena [cite: 613]
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }

    // Remove objetos mortos varrendo o array de trás para frente [cite: 614-616]
    for (int i = objectArray.size() - 1; i >= 0; i--) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i); 
        }
    }
}

// Renderiza a cena
void State::Render() {
    // Percorre o array chamando a função Render de todos os objetos [cite: 618]
    for (unsigned i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Render();
    }
}

// Informa ao Game Loop se deve encerrar o jogo
bool State::QuitRequested() {
    return quitRequested;
}