#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {} // Construtor inicializando em zero
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::Add(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
Vec2 Vec2::Sub(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
Vec2 Vec2::Mul(const float scalar) const { return Vec2(x * scalar, y * scalar); }

float Vec2::Magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::Normalize() const {
    float mag = Magnitude();
    if (mag == 0) return Vec2(0, 0);
    return Vec2(x / mag, y / mag);
}

float Vec2::Distance(const Vec2& other) const {
    return Sub(other).Magnitude();
}

void Vec2::Rotate(float angle) {
    float newX = x * std::cos(angle) - y * std::sin(angle); // 
    float newY = y * std::cos(angle) + x * std::sin(angle); // 
    x = newX;
    y = newY;
}