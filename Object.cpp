#include "Object.h"

#include <numbers>

float vector_product(vec2<float> a, vec2<float> b) { 
    return a.x * b.y - a.y * b.x;
} 
float scalar_product(vec2<float> a, vec2<float> b) {
    return  b.x * a.x + b.y * a.y;
}
float angle_between(vec2<float> a, vec2<float> b) {
#if 0
    if (a.sq_length() * b.sq_length() < 1e-4)
        return 0.f;

    float dot = scalar_product(a, b) / (b.length() * a.length());
    if (std::abs(dot) > 1.f)
        return std::numbers::pi_v<float>;
    const auto acos = std::acos(dot);
    if (vector_product(a, b) > 0.f)
        return acos;
    return -acos;
#else
    if ((a.sq_length() * b.sq_length()) != 0) {
        float dot = scalar_product(a, b) / (b.length() * a.length());
        if (dot > 1) dot = 1.f;
        else if (dot < -1) dot = -1.f;
        if (a.x * b.y - a.y * b.x > 0) {
            return std::acos(dot);
        }
        else {
            return -(std::acos(dot));
        }
    }
    else return 0.f;
#endif
}

static bool point_in_triangle(vec2<int> vec, const Triangle& t) {
    vec2<float> v(static_cast<float>(vec.x), static_cast<float>(vec.y));
    float a = vector_product(t.p1 - v, t.p2 - t.p1);
    float b = vector_product(t.p2 - v, t.p3 - t.p2);
    float c = vector_product(t.p3 - v, t.p1 - t.p3);
    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        return true;
    return false;

}

void Triangle::draw(BuffTy buffer) const {
    int y_max = static_cast<int>(std::max(std::max(p1.y, p2.y), std::max(p3.y, p2.y)));
    int x_max = static_cast<int>(std::max(std::max(p1.x, p2.x), std::max(p3.x, p2.x)));
    int y_min = static_cast<int>(std::min(std::min(p1.y, p2.y), std::min(p3.y, p2.y)));
    int x_min = static_cast<int>(std::min(std::min(p1.x, p2.x), std::min(p3.x, p2.x)));
    
    for (int j = y_min; j <= y_max; ++j) {
        for (int i = x_min; i <= x_max; ++i) {
            if (point_in_triangle(vec2<int>(i, j), *this)) {
                buffer[j][i] = color.data;
            }
        }
    }
}

vec2<float> Triangle::getCenter() const {
    return (p1 + p2 + p3) * 0.333f;
}

