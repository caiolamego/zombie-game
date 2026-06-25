#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Rect.h"

class Component;

class GameObject {
public:
  GameObject();
  ~GameObject();

  void Start();
  void Update(float dt);
  void Render();

  bool IsDead() const { return isDead; }
  void RequestDelete() { isDead = true; }

  void AddComponent(Component* cpt);
  void RemoveComponent(Component* cpt);

  void NotifyCollision(GameObject& other);

  template <typename T>
  T* GetComponent();

  Rect box;
  double angleDeg; 

private:
  std::vector<Component*> components;
  bool isDead;
  bool started;
};

#include <type_traits>
#include <typeinfo>
#include "Component.h"

template <typename T>
T* GameObject::GetComponent() {
  for (auto* c : components) {
    if (auto* casted = dynamic_cast<T*>(c)) return casted;
  }
  return nullptr;
}

#endif
