#pragma once
#include <cmath>

class Vec2 {
public:
    float x, y;

    Vec2();
    Vec2(float x, float y);

    Vec2 Add(const Vec2& other) const;
    Vec2 Sub(const Vec2& other) const;
    Vec2 Mul(const float scalar) const;
    float Magnitude() const;
    Vec2 Normalize() const;
    float Distance(const Vec2& other) const;
    void Rotate(float angle);
};