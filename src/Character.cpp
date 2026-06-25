#include "Character.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Animation.h"
#include "GameObject.h"
#include "State.h"
#include "Game.h"
#include "Gun.h"
#include "Bullet.h"
#include "Zombie.h"
#include "Camera.h"
#include "Collider.h"

Character* Character::player = nullptr;

Character::Character(GameObject& associated, const std::string& sprite)
    : Component(associated),
      gun(),
      taskQueue(),
      speed(0, 0),
      linearSpeed(220.0f),
      hp(100),
      deathTimer(),
      hit(false),          
      hitTimer() ,
      spritePath(sprite),
      damageCooldown(),
      damageInterval(0.6f),
      hitSound("recursos/audio/Hit1.wav"),
      deathSound("recursos/audio/Dead.wav") {

    auto* sr = new SpriteRenderer(associated, spritePath, 3, 4);
    associated.AddComponent(sr);

    auto* anim = new Animator(associated);
    anim->AddAnimation("idle",   Animation(9, 9, 0.0f));
    anim->AddAnimation("walking",Animation(0, 2, 0.12f));
    anim->AddAnimation("dead",   Animation(11, 11, 0.0f));
    anim->SetAnimation("idle");
    associated.AddComponent(anim);

    if (!Character::player) {
        Character::player = this;
    }
}

Character::~Character() {
    if (Character::player == this) {
        Character::player = nullptr;
    }
}

void Character::Start() {
    // arma
    auto* gunGO = new GameObject();
    gunGO->box.x = associated.box.x;
    gunGO->box.y = associated.box.y;

    auto sp = Game::GetInstance().GetState().AddObject(gunGO);
    gun = sp;

    auto locked = sp.lock();
    if (locked) {
        locked->AddComponent(
            new Gun(*locked, Game::GetInstance().GetState().GetObjectPtr(&associated))
        );
    }

    auto* col = new Collider(associated);
    col->SetScale(Vec2(0.5f, 0.8f));
    col->SetOffset(Vec2(0.0f, 8.0f));
    associated.AddComponent(col);
  }

void Character::Issue(const Command& task) {
    taskQueue.push(task);
}

void Character::Update(float dt) {
    damageCooldown.Update(dt);
    hitTimer.Update(dt);

    // Se estiver morto: não anda, não atira, só animação de morte e timer
    if (hp <= 0) {
        while (!taskQueue.empty()) taskQueue.pop();

        if (auto* a = associated.GetComponent<Animator>()) {
            a->SetAnimation("dead");
        }

        deathTimer.Update(dt);
        if (deathTimer.Get() > 3.0f) {
            associated.RequestDelete();
        }
        return;
    }

    // se já passou o tempo de hit, desliga o estado
    if (hit && hitTimer.Get() > 0.3f) { // 0.3s visível
        hit = false;
    }

    Vec2 moveIntent(0, 0);

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
        speed = Vec2(0, 0);
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

void Character::NotifyCollision(GameObject& other) {
    if (hp <= 0) return;

    // 1) Colisão com Bullet
    if (auto* bullet = other.GetComponent<Bullet>()) {
        bool shouldTakeDamage = false;

        if (bullet->TargetsPlayer()) {
            // bala de NPC -> só acerta player
            shouldTakeDamage = (this == Character::player);
        } else {
            // bala do player -> só acerta quem NÃO é o player (NPC, zumbi, etc.)
            shouldTakeDamage = (this != Character::player);
        }

        if (shouldTakeDamage && damageCooldown.Get() >= damageInterval) {
            hp -= bullet->GetDamage();
            hit = true;
            hitTimer.Restart();
            damageCooldown.Restart();

            if (hp > 0) {
                if (hitSound.IsOpen()) hitSound.Play(1);
            } else {
                hp = 0;
                if (deathSound.IsOpen()) deathSound.Play(1);
                deathTimer.Restart();

                if (this == Character::player) {
                    Camera::GetInstance().Unfollow();
                }
            }

            other.RequestDelete();
        }
    }

    // 2) Dano por contato com zumbi (só no player para simplificar)
    if (auto* zombie = other.GetComponent<Zombie>()) {
        (void)zombie; // só para tirar warning

        if (this == Character::player && damageCooldown.Get() >= damageInterval) {
            const int touchDamage = 10;
            hp -= touchDamage;
            damageCooldown.Restart();

            if (hp > 0) {
                if (hitSound.IsOpen()) hitSound.Play(1);
            } else {
                hp = 0;
                if (deathSound.IsOpen()) deathSound.Play(1);
                deathTimer.Restart();
                Camera::GetInstance().Unfollow();
            }
        }
    }
}
