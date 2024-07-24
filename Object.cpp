#include "Object.h"

#include <numbers>


float angle_between(const vec2<float>& a, const vec2<float>& b) {
    if ((a.sq_length() * b.sq_length()) < FLOAT_PRECISE)
        return 0.f;
    float dot = (a.dot(b)) / (b.length() * a.length());
    if (std::abs(dot) >= 1.f)
        return 0.f;
    const auto acos = std::acos(dot);
    if ((a.cross(b)) > 0.f)
        return acos;
    return -acos;
}

static bool point_in_triangle(const vec2<int>& vec, const Triangle2& t) {
    vec2<float> v(static_cast<float>(vec.x), static_cast<float>(vec.y));
    float a = (t.v1 - v).cross(t.v2 - t.v1);
    float b = (t.v2 - v).cross(t.v3 - t.v2);
    float c = (t.v3 - v).cross(t.v1 - t.v3);
    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        return true;
    return false;
}
static bool point_in_triangle(const vec2<int>& vec, vec2<float> p1, vec2<float> p2, vec2<float> p3) {
    vec2<float> v(static_cast<float>(vec.x), static_cast<float>(vec.y));
    float a = (p1 - v).cross(p2 - p1);
    float b = (p2 - v).cross(p3 - p2);
    float c = (p3 - v).cross(p1 - p3);
    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        return true;
    return false;
}
static bool point_in_triangle2(const vec2<int>& vec, const Triangle2& t) {
    vec2<float> v(static_cast<float>(vec.x) - t.position.x, static_cast<float>(vec.y) - t.position.y);
    float a = (t.v1 - v).cross(t.v2 - t.v1);
    float b = (t.v2 - v).cross(t.v3 - t.v2);
    float c = (t.v3 - v).cross(t.v1 - t.v3);
    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        return true;
    return false;
}


void Triangle2::draw(BuffTy buffer) const {
    vec2<int> p(static_cast<int>(position.x), static_cast<int>(position.y));
    vec2<int> y = get_min_max(v1.y, v2.y, v3.y) + vec2<int>(p.y, p.y);
    vec2<int> x = get_min_max(v1.x, v2.x, v3.x) + vec2<int>(p.x, p.x);
    for (int j = y.x; j <= y.y; ++j)
        for (int i = x.x; i <= x.y; ++i)
            if (point_in_triangle2(vec2<int>(i, j), *this)) {
                if (i<0 || i > SCREEN_WIDTH || j < 0 || j > SCREEN_HEIGHT)
                    continue;
                buffer[j][i] = color.data;
            }
}

void Triangle2::rotate(float phi) {
    Matrix m(phi);
    v1 = m * v1;
    v2 = m * v2;
    v3 = m * v3;
}


vec2<int> get_min_max(float a, float b, float c) {
    float max = a, min = a;
    if (b > max) max = b;
    else min = b;
    if (c > max) max = c;
    else if (c < min) min = c;
    return vec2<int>(static_cast<int>(min), static_cast<int>(max));
}

float random(float a, float b) {
    int ia = static_cast<int>(a);
    int ib = static_cast<int>(b);
    return static_cast<float>(ia + std::rand() % (std::abs(ia) + ib));
}


