#pragma once
#include "Component.h"
#include "Sprite.h"

class SpriteRenderer : public Component { // herda de Component [cite: 682]
private:
    Sprite sprite; // [cite: 698]

public:
    SpriteRenderer(GameObject& associated); // [cite: 683]
    SpriteRenderer(GameObject& associated, std::string file, int frameCountW = 1, int frameCountH = 1); // [cite: 684-689]

    void Open(std::string file); // [cite: 690-691]
    void SetFrameCount(int frameCountW, int frameCountH); // [cite: 693-694]
    void SetFrame(int frame); // [cite: 697]

    void Update(float dt) override; // [cite: 695]
    void Render() override; // [cite: 696]
};