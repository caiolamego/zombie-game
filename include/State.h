#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Music.h"

class State {
public:
    State();
    ~State();

    bool QuitRequested() const;
    void LoadAssets();
    void Update(float dt);
    void Render();
    
    void Start();
    std::weak_ptr<GameObject> AddObject(GameObject* go);
    std::weak_ptr<GameObject> GetObjectPtr(GameObject* go);

private:
    std::vector<std::shared_ptr<GameObject>> objectArray;
    Music music;
    bool quitRequested;
    bool started;
};