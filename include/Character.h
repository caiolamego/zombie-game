#ifndef CHARACTER_H
#define CHARACTER_H

#include "Component.h"
#include "Timer.h"
#include "Vec2.h"
#include <queue>
#include <memory>
#include <string>

class Character : public Component {
public:
  enum class CommandType { MOVE, SHOOT };
  struct Command {
    CommandType type;
    Vec2 pos;
    Command(CommandType t = CommandType::MOVE, float x=0, float y=0)
    : type(t), pos(x,y) {}
  };

  explicit Character(GameObject& associated, const std::string& spritePath);
  ~Character();

  void Start() override;
  void Update(float dt) override;
  void Render() override {}

  void Issue(const Command& task);

  static Character* player;

private:
  std::weak_ptr<GameObject> gun;
  std::queue<Command> taskQueue;
  Vec2  speed;
  float linearSpeed;
  int   hp;
  Timer deathTimer;
  std::string spritePath;
};

#endif
