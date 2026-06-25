#ifndef ANIMATION_H
#define ANIMATION_H

#define INCLUDE_SDL
#include "SDL_include.h"

struct Animation {
  int   frameStart;
  int   frameEnd;
  float frameTime;
  SDL_RendererFlip flip;

  Animation(int fs=0, int fe=0, float ft=0.0f, SDL_RendererFlip f = SDL_FLIP_NONE)
    : frameStart(fs), frameEnd(fe), frameTime(ft), flip(f) {}
};
#endif
