#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Component.h"
#include "Sound.h"
#include "Timer.h"

class Zombie : public Component {
public:
  explicit Zombie(GameObject& associated);

  void Damage(int damage);
  void Update(float dt) override;
  void Render() override;

private:
  int   hitpoints;
  bool  died;
  bool  hit;
  Sound deathSound;
  Sound hitSound;
  Timer hitTimer;
  Timer deathTimer;
};

#endif
