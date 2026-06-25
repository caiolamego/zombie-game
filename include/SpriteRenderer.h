#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "Component.h"
#include "Sprite.h"
#include <string>

class SpriteRenderer : public Component {
public:
  explicit SpriteRenderer(GameObject& associated);
  SpriteRenderer(GameObject& associated, const std::string& file, int frameCountW = 1, int frameCountH = 1);

  void Open(const std::string& file);
  void SetFrameCount(int frameCountW, int frameCountH);
  void SetFrame(int frame);
  void SetFrame(int frame, SDL_RendererFlip flip);

  void SetScale(float scaleX, float scaleY);

  void Update(float dt) override;
  void Render() override;

  Sprite& GetSprite() { return sprite; }

  void SetCameraFollower(bool v) { cameraFollower = v; }

private:
  Sprite sprite;
  bool cameraFollower;
};

#endif
