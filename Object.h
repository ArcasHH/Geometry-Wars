#pragma once
#include "Engine.h"
#include <vector>
#include <cmath>

using BuffTy = decltype(buffer);
union Color final {
    struct {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    };
    uint32_t data;
    Color() : data(0x000000ff) {}
    Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) : r{ R }, g{ G }, b{ B }, a{ A } { }
    Color& operator=(const Color&) = default;
};

template< typename T >
struct vec2 final {
    T x{}, y{};
    // –азобратьс€ с бинарными\унарными операторами
    vec2 operator+ (vec2 a) const {
        return vec2(x + a.x, y + a.y);
    }
    vec2& operator+= (vec2 a) {
        x += a.x;
        y += a.y;
        return *this;
    }
    vec2 operator- (vec2 a)const {
        return vec2(x - a.x, y - a.y);
    }
    vec2& operator-= (vec2 a) {
        x -= a.x;
        y -= a.y;
        return *this;
    }
    vec2 operator* (vec2 a) const {
        return vec2(x * a.x, y * a.y);
    }
    vec2& operator*= (vec2 a) {
        x *= a.x;
        y *= a.y;
        return *this;
    }
    vec2 operator*(T a) const {
        return vec2(a * x, a * y);
    }
    vec2& operator*=(T a) {
        x *= a;
        y *= a;
        return *this;
    }

    bool operator==(vec2 a) const {
        return (x == a.x && y == a.y);
    }

    vec2() = default;
    vec2(T a, T b) : x(a), y(b) {}
    vec2& operator=(const vec2&) = default;

    float sq_length() {
        return x * x + y * y;
    }
};


struct Triangle final {
    vec2<float> p1, p2, p3;
    Color color;

    Triangle() = default;
    Triangle(vec2<float> pos1, vec2<float> pos2, vec2<float> pos3, Color c) :
        p1{ pos1 }, p2{ pos2 }, p3{ pos3 }, color{ c } {}

    Triangle& operator=(const Triangle& Other) = default;
    void draw(BuffTy buffer);

    vec2<float> getCenter() const;

private:
    void reorganize();  // sort points by y coord
    void draw_horizontal_segment(BuffTy buffer, float xpos1, float xpos2, int y) const;
    void draw_top(BuffTy buffer, vec2<float>& p1, vec2<float>& p2, vec2<float>& p3) const;
    void draw_bottom(BuffTy buffer, vec2<float>& p1, vec2<float>& p2, vec2<float>& p3) const;
};

struct Rectangle final{
    vec2<float> size;
    vec2<float> center_pos;
    Color color;
    Triangle t1, t2;

    Rectangle() = default;
    Rectangle(vec2<float> Center_Position, vec2<float> Size, Color c) : center_pos{ Center_Position }, size{ Size }, color{ c } {
        vec2<float> lt = Center_Position - (Size * 0.5);
        vec2<float> rb = Center_Position + (Size * 0.5);
        vec2<float> rt(rb.x, lt.y);
        vec2<float> lb(lt.x, rb.y);
        t1 = Triangle(lt, rb, rt, c);
        t2 = Triangle(lt, rb, lb, c);
    }
    void set_by_lt_rb(vec2<float> lt, vec2<float> rb, Color c);
    void draw(BuffTy buffer);
    
};
