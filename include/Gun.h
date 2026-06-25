#ifndef GUN_H
#define GUN_H

#include "Component.h"
#include "Sound.h"
#include "Timer.h"
#include "Vec2.h"          // <-- necessário para Vec2
#include <memory>

class Gun : public Component {
public:
  Gun(GameObject& associated, std::weak_ptr<GameObject> character);

  void Update(float dt) override;
  void Render() override {}

  void Shoot(Vec2 target); // agora Vec2 está declarado

private:
  Sound shotSound;
  Sound reloadSound;

  int   cooldownState; // 0 pronto | 1 pós-tiro | 2 recarregando | 3 finalizando
  Timer cdTimer;

  std::weak_ptr<GameObject> character;
  float angle; // radianos
};

#endif
