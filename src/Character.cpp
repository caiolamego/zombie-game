#include "Character.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Animation.h"
#include "GameObject.h"
#include "State.h"
#include "Game.h"
#include "Gun.h"

Character* Character::player = nullptr;

Character::Character(GameObject& associated, const std::string& sprite) 
    : Component(associated), gun(), taskQueue(), speed(0,0), linearSpeed(220.0f), 
      hp(100), deathTimer(), spritePath(sprite) {
      
    auto* sr = new SpriteRenderer(associated, spritePath, 3, 4);
    associated.AddComponent(sr);
    
    auto* anim = new Animator(associated);
    anim->AddAnimation("idle",    Animation(9, 9, 0.0f));
    anim->AddAnimation("walking", Animation(0, 2, 0.12f));
    anim->AddAnimation("dead",    Animation(11, 11, 0.0f));
    anim->SetAnimation("idle");
    associated.AddComponent(anim);
    
    if (!Character::player) Character::player = this;
}

Character::~Character() {
    if (Character::player == this) Character::player = nullptr;
}

void Character::Start() {
    auto* gunGO = new GameObject();
    gunGO->box.x = associated.box.x;
    gunGO->box.y = associated.box.y;
    auto sp = Game::GetInstance().GetState().AddObject(gunGO);
    gun = sp;
    
    auto locked = sp.lock();
    if (locked) {
        locked->AddComponent(new Gun(*locked, Game::GetInstance().GetState().GetObjectPtr(&associated)));
    }
}

void Character::Issue(const Command& task) {
    taskQueue.push(task);
}

void Character::Update(float dt) {
    Vec2 moveIntent(0,0);
    
    while (!taskQueue.empty()) {
        const auto task = taskQueue.front();
        if (task.type == CommandType::MOVE) {
            moveIntent = moveIntent + task.pos; 
        } else if (task.type == CommandType::SHOOT) {
            if (auto g = gun.lock()) {
                if (auto* gunC = g->GetComponent<Gun>()) {
                    gunC->Shoot(task.pos);
                }
            }
        }
        taskQueue.pop();
    }
    
    if (moveIntent.x != 0.0f || moveIntent.y != 0.0f) {
        Vec2 dir = moveIntent.Normalized();
        speed = dir * linearSpeed;
    } else {
        speed = Vec2(0,0);
    }
    
    associated.box.x += speed.x * dt;
    associated.box.y += speed.y * dt;
    
    if (hp > 0) {
        if (speed.Magnitude() > 1.0f) {
            if (auto* a = associated.GetComponent<Animator>()) a->SetAnimation("walking");
        } else {
            if (auto* a = associated.GetComponent<Animator>()) a->SetAnimation("idle");
        }
    } else {
        if (auto* a = associated.GetComponent<Animator>()) a->SetAnimation("dead");
        deathTimer.Update(dt);
        if (deathTimer.Get() > 3.0f) {
            associated.RequestDelete();
        }
    }
}

void Character::Render() {}