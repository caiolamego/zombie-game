#include "Collision.h"
#include "Vec2.h"
#include <cmath>

static void GetRectPoints(const Rect& r, float angle, Vec2* pts) {
    Vec2 center = r.Center();
    Vec2 half(r.w * 0.5f, r.h * 0.5f);

    pts[0] = Vec2(-half.x, -half.y);
    pts[1] = Vec2( half.x, -half.y);
    pts[2] = Vec2( half.x,  half.y);
    pts[3] = Vec2(-half.x,  half.y);

    float cs = std::cos(angle);
    float sn = std::sin(angle);

    for (int i = 0; i < 4; ++i) {
        float x = pts[i].x;
        float y = pts[i].y;
        pts[i].x = x * cs - y * sn + center.x;
        pts[i].y = x * sn + y * cs + center.y;
    }
}

static void Project(const Vec2* pts, const Vec2& axis, float& min, float& max) {
    float dot = pts[0].x * axis.x + pts[0].y * axis.y;
    min = max = dot;
    for (int i = 1; i < 4; ++i) {
        dot = pts[i].x * axis.x + pts[i].y * axis.y;
        if (dot < min) min = dot;
        if (dot > max) max = dot;
    }
}

static bool OverlapOnAxis(const Vec2* aPts, const Vec2* bPts, const Vec2& axis) {
    if (axis.x == 0.0f && axis.y == 0.0f) return true;

    Vec2 n = axis.Normalized();
    float minA, maxA, minB, maxB;
    Project(aPts, n, minA, maxA);
    Project(bPts, n, minB, maxB);

    return !(maxA < minB || maxB < minA);
}

bool IsColliding(const Rect& a, const Rect& b, float angleOfA, float angleOfB) {
    Vec2 aPts[4];
    Vec2 bPts[4];

    GetRectPoints(a, angleOfA, aPts);
    GetRectPoints(b, angleOfB, bPts);

    Vec2 axes[4] = {
        aPts[1] - aPts[0],
        aPts[3] - aPts[0],
        bPts[1] - bPts[0],
        bPts[3] - bPts[0]
    };

    for (int i = 0; i < 4; ++i) {
        if (!OverlapOnAxis(aPts, bPts, axes[i])) {
            return false;
        }
    }
    return true;
}
