#pragma once
#include <string>
#include <unordered_map>
#include "Component.h"
#include "Animation.h"

class Animator : public Component { // herda de Component [cite: 745]
private:
    std::unordered_map<std::string, Animation> animations; // [cite: 755-757]
    int frameStart, frameEnd; // [cite: 758-759]
    float frameTime; // [cite: 760]
    int currentFrame; // [cite: 761]
    float timeElapsed; // [cite: 762]

public:
    Animator(GameObject& associated); // [cite: 746-748]

    void AddAnimation(std::string name, Animation anim); // [cite: 753-754]
    void SetAnimation(std::string name); // [cite: 751-752]

    void Update(float dt) override; // [cite: 749]
    void Render() override; // [cite: 750]
};