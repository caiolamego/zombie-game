#ifndef STATE_H
#define STATE_H

#include <vector>
#include <memory>
#include "Music.h"
#include "GameObject.h"

class State {
public:
  State();
  ~State();

  bool QuitRequested() const;

  void LoadAssets();
  void Start();
  void Update(float dt);
  void Render();

  std::weak_ptr<GameObject> AddObject(GameObject* go);
  std::weak_ptr<GameObject> GetObjectPtr(GameObject* go) const;

private:
  bool   started;
  std::vector<std::shared_ptr<GameObject>> objectArray;
  Music  music;
  bool   quitRequested;
};

#endif
