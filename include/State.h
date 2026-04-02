#pragma once
#include "Sprite.h"
#include "Music.h"

class State {
private:
    Sprite bg;
    Music music;
    bool quitRequested;

public:
    State();
    bool QuitRequested();
    void LoadAssets();
    void Update(float dt);
    void Render();
};