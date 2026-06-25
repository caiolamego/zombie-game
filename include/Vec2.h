#ifndef VEC2_H
#define VEC2_H

#include <cmath>

struct Vec2 {
  float x, y;

  Vec2() : x(0), y(0) {}
  Vec2(float x_, float y_) : x(x_), y(y_) {}

  Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
  Vec2 operator-(const Vec2& o) const { return Vec2(x - o.x, y - o.y); }
  Vec2 operator*(float s) const { return Vec2(x * s, y * s); }

  float Magnitude() const { return std::sqrt(x*x + y*y); }

  Vec2 Normalized() const {
    float m = Magnitude();
    return (m > 0.0f) ? Vec2(x/m, y/m) : Vec2(0,0);
  }

  static float Distance(const Vec2& a, const Vec2& b) {
    return (a - b).Magnitude();
  }

  float Angle() const { return std::atan2(y, x); }

  Vec2 Rotated(float theta) const {
    float cs = std::cos(theta), sn = std::sin(theta);
    return Vec2(x*cs - y*sn, y*cs + x*sn);
  }
};

#endif
