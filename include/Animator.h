#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

class Animator : public Component {
public:
  explicit Animator(GameObject& associated);

  void Update(float dt) override;
  void Render() override;

  void SetAnimation(const std::string& name);
  void AddAnimation(const std::string& name, const Animation& anim);

  std::string current; 

private:
  std::unordered_map<std::string, Animation> animations;
  int   frameStart;
  int   frameEnd;
  float frameTime;
  int   currentFrame;
  float timeElapsed;
};

#endif
