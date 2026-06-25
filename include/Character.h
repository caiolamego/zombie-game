#pragma once
#include "Component.h"
#include "Timer.h"
#include "Vec2.h"
#include <string>
#include <queue>
#include <memory>

class Character : public Component {
public:
    enum class CommandType { MOVE, SHOOT };

    class Command {
    public:
        CommandType type;
        Vec2 pos;
        Command(CommandType t, float x, float y) : type(t), pos(x, y) {}
    };

    Character(GameObject& associated, const std::string& sprite);
    ~Character();

    void Start() override;
    void Update(float dt) override;
    void Render() override;
    void Issue(const Command& task);

    static Character* player;

private:
    std::weak_ptr<GameObject> gun;
    std::queue<Command> taskQueue;
    Vec2 speed;
    float linearSpeed;
    int hp;
    Timer deathTimer;
    std::string spritePath;
};