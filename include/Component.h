#pragma once

class GameObject;

class Component {
public:
    explicit Component(GameObject& associated) : associated(associated) {}
    virtual ~Component() = default;

    virtual void Start() {}

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    // chamada quando o GameObject colide com outro
    virtual void NotifyCollision(GameObject& other) {}

    // getters 
    GameObject& GetGameObject()             { return associated; }
    const GameObject& GetGameObject() const { return associated; }

protected:
    GameObject& associated;
};
