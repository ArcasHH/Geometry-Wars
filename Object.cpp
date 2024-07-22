#include "Object.h"


float vec_mul(vec2<float> a, vec2<float> b) {
    return a.x * b.y - a.y * b.x;
}
bool point_in_triangle(vec2<int> vec, Triangle& t) {
    vec2<float> v (static_cast<float>(vec.x), static_cast<float>(vec.y));
    float a = vec_mul(t.p1 - v, t.p2 - t.p1);
    float b = vec_mul(t.p2 - v, t.p3 - t.p2);
    float c = vec_mul(t.p3 - v, t.p1 - t.p3);
    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        return true;
    return false;

}
/*
float seg_ratio(vec2<float>& a, vec2<float>& b) {
    if((a.y - b.y)!=0)
        return  ((a.x - b.x) / (a.y - b.y));
    return 0;
}

void Triangle:: reorganize() {   // sort points by y coord:      // p1 - min by y   // p2 - middle by y    // p3 - max by y
    if (p1.y > p3.y)
        std::swap(p1, p3);
    if (p2.y > p3.y)
        std::swap(p2, p3);
    if (p1.y > p2.y)
        std::swap(p1, p2);
}
void Triangle::draw_horizontal_segment(BuffTy buffer, float xpos1, float xpos2, int y) const {
    int x1 = static_cast<int> (xpos1);
    int x2 = static_cast<int> (xpos2);
    if (y < 0) y = 0;
    if (y > SCREEN_HEIGHT) y = SCREEN_HEIGHT;
    if (x2 < x1)
        std::swap(x1, x2);
    for (int i = x1; i < x2; ++i) {
        int j = i;
        if (j < 0) j = 0;
        if (j > SCREEN_WIDTH) j = SCREEN_WIDTH;
        buffer[y][j] = color.data;
    }

}

void Triangle::draw_top(BuffTy buffer, vec2<float>& p1, vec2<float>& p2, vec2<float>& p3) const { // case when p1.y == p2.y
    vec2<float> sr(seg_ratio(p3, p1), seg_ratio(p3, p2));//wrong interpretation (This is not a vector)
    vec2<float> curr(p3.x, p3.x);                      //wrong interpretation (This is not a vector)
    int p3y = static_cast<int> (p3.y);
    int p1y = static_cast<int> (p1.y);
    for (int scanlineY = p3y; scanlineY > p1y; --scanlineY) {
        draw_horizontal_segment(buffer, curr.x, curr.y, scanlineY);
        curr -= sr;
    }
}
void Triangle::draw_bottom(BuffTy buffer, vec2<float>& p1, vec2<float>& p2, vec2<float>& p3) const { // case when p3.y == p2.y
    vec2<float> sr(seg_ratio(p2, p1), seg_ratio(p3, p1));//wrong interpretation (This is not a vector)
    vec2<float> curr(p1.x, p1.x);                       //wrong interpretation (This is not a vector)
    int p2y = static_cast<int> (p2.y);
    int p1y = static_cast<int> (p1.y);
    for (int scanline = p1y; scanline <= p2y; ++scanline) {
        draw_horizontal_segment(buffer, curr.x, curr.y, scanline);
        curr += sr;
    }
}
void Triangle::draw(BuffTy buffer) {
    
    reorganize();
    if (p1.y == p2.y) {
        draw_top(buffer, p1, p2, p3);
        return;
    }
    else if (p3.y == p2.y) {
        draw_bottom(buffer, p1, p2, p3);
        return;
    }
    else {
        vec2<float> p4((p1.x + ((p2.y - p1.y) / (p3.y - p1.y)) * (p3.x - p1.x)), p2.y);
        draw_bottom(buffer, p1, p2, p4);
        draw_top(buffer, p2, p4, p3);
    }
    
}
*/

void Triangle::draw(BuffTy buffer) {
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
    vec2<float> p5;
    p5 = (p1 + p3) * (0.5);
    return  p5 + ((p2 - p5) * (1 / 3)); ;
}

