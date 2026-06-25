#ifndef RECT_H
#define RECT_H

#include "Vec2.h"

struct Rect {
  float x, y, w, h;

  Rect() : x(0), y(0), w(0), h(0) {}
  Rect(float x_, float y_, float w_, float h_) : x(x_), y(y_), w(w_), h(h_) {}

  Vec2 Center() const { return Vec2(x + w*0.5f, y + h*0.5f); }
  bool Contains(float px, float py) const { return px >= x && py >= y && px <= x+w && py <= y+h; }

  Rect operator+(const Vec2& v) const { return Rect(x + v.x, y + v.y, w, h); }

  static float CenterDistance(const Rect& a, const Rect& b) {
    return Vec2::Distance(a.Center(), b.Center());
  }
};

#endif
