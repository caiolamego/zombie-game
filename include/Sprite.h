#ifndef SPRITE_H
#define SPRITE_H

#define INCLUDE_SDL
#include "SDL_include.h"
#include <string>
#include "Vec2.h"

class Sprite {
public:
  Sprite();
  explicit Sprite(const std::string& file, int frameCountW = 1, int frameCountH = 1);
  ~Sprite();

  void Open(const std::string& file);
  void SetClip(int x, int y, int w, int h);

  void Render(int x, int y);
  void Render(int x, int y, int w, int h);
  void RenderEx(int x, int y, int w, int h, double angleDeg);

  void SetFrame(int frame);
  void SetFrameCount(int frameCountW, int frameCountH);

  void SetScale(float sx, float sy);
  Vec2 GetScale() const { return scale; }
  void SetFlip(SDL_RendererFlip f);

  int  GetWidth()  const { return int(frameW * ((scale.x != 0.0f)? scale.x : 1.0f)); }
  int  GetHeight() const { return int(frameH * ((scale.y != 0.0f)? scale.y : 1.0f)); }
  bool IsOpen()    const { return texture != nullptr; }

private:
  void RecalcFrameSize();

  SDL_Texture* texture;
  int sheetW, sheetH;
  int frameW, frameH;
  int framesW, framesH;
  SDL_Rect clipRect;

  SDL_RendererFlip flip;
  Vec2 scale;
};

#endif
