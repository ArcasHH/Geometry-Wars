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

static bool point_in_triangle(const vec2<int>& vec, const Triangle& t) {
    vec2<float> v(static_cast<float>(vec.x), static_cast<float>(vec.y));  
    float a = (t.p1 - v).cross(t.p2 - t.p1);
    float b = (t.p2 - v).cross(t.p3 - t.p2);
    float c = (t.p3 - v).cross(t.p1 - t.p3);
    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        return true;
    return false;

}

void Triangle::draw(BuffTy buffer) const {
    vec2<int> y = get_min_max(p1.y, p2.y, p3.y);
    vec2<int> x = get_min_max(p1.x, p2.x, p3.x);
    for (int j = y.x; j <= y.y; ++j) 
        for (int i = x.x; i <= x.y; ++i)
            if (point_in_triangle(vec2<int> (i, j), *this))
                buffer[j][i] = color.data;
}
void Triangle::rotate(float phi) {
    Matrix m(phi);
    vec2<float> p0 = getCenter();
    p1 = m * (p1 - p0) + p0;
    p2 = m * (p2 - p0) + p0;
    p3 = m * (p3 - p0) + p0;
}

vec2<float> Triangle::getCenter() const {
    return (p1 + p2 + p3) * 0.333f;
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
