#ifndef CAMERA_H
#define CAMERA_H

#include "Vec2.h"

class GameObject;

class Camera {
public:
  static Camera& GetInstance();

  void Follow(GameObject* newFocus);
  void Unfollow();

  void Update(float dt); 

  Vec2 pos;
  Vec2 speed; 

private:
  Camera();
  ~Camera();

  GameObject* focus;
};

#endif
