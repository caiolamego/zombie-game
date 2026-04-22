#pragma once
#include <vector>
#include <memory>
#include "Music.h"
#include "Sprite.h"
#include "GameObject.h"

class State {
private:
    Music music;
    bool quitRequested;
    std::vector<std::unique_ptr<GameObject>> objectArray;

public:
    State();
    ~State();
    
    bool QuitRequested();
    void LoadAssets();
    void Update(float dt);
    void Render();
    
    void AddObject(GameObject* go);
};