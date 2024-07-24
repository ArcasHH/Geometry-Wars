#include "Math.h"

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


bool point_in_triangle(const vec2<int>& vec, vec2<float> p1, vec2<float> p2, vec2<float> p3) {
    vec2<float> v(static_cast<float>(vec.x), static_cast<float>(vec.y));
    float a = (p1 - v).cross(p2 - p1);
    float b = (p2 - v).cross(p3 - p2);
    float c = (p3 - v).cross(p1 - p3);
    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        return true;
    return false;
}

void rotateSprite(float phi, vec2<float>& v1, vec2<float>& v2, vec2<float>& v3) {
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


