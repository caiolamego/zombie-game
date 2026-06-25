#include "Animator.h"
#include "GameObject.h"
#include "SpriteRenderer.h"

Animator::Animator(GameObject& associated)
: Component(associated),
  current(""),
  frameStart(0), frameEnd(0), frameTime(0.0f),
  currentFrame(0), timeElapsed(0.0f) {}

void Animator::Update(float dt) {
  if (frameTime == 0.0f) return;

  timeElapsed += dt;
  if (timeElapsed > frameTime) {
    timeElapsed -= frameTime;
    int newFrame = currentFrame + 1;
    if (newFrame > frameEnd) newFrame = frameStart;

    if (newFrame != currentFrame) {
      currentFrame = newFrame;
      if (auto* sr = associated.GetComponent<SpriteRenderer>()) {
        sr->SetFrame(currentFrame); 
      }
    }
  }
}

void Animator::Render() { }

void Animator::SetAnimation(const std::string& name) {
  if (current == name) return;

  auto it = animations.find(name);
  if (it == animations.end()) return;

  const Animation& a = it->second;
  current      = name;
  frameStart   = a.frameStart;
  frameEnd     = a.frameEnd;
  frameTime    = a.frameTime;
  currentFrame = frameStart;
  timeElapsed  = 0.0f;

  if (auto* sr = associated.GetComponent<SpriteRenderer>()) {
    sr->SetFrame(currentFrame, a.flip);
  }
}

void Animator::AddAnimation(const std::string& name, const Animation& anim) {
  if (animations.find(name) == animations.end()) {
    animations.emplace(name, anim);
  }
}
