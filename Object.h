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
    // ����������� � ���������\�������� �����������
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

    float sq_length() const {
        return x * x + y * y;
    }
    float length() const {
        return std::sqrt(sq_length());
    }
    void normalize() {
        x = x / length();
        y = y / length();
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
/*
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

*/

struct Matrix {
    float data[2][2];
    Matrix() = default;
    Matrix(float phi) : data{std::cos(phi), -std::sin(phi) ,std::sin(phi) ,std::cos(phi) } {}
    Matrix(float x11, float x12, float x21, float x22) : data{x11,x12,x21,x22} { }

    Matrix operator* (float a)  {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                data[i][j] *= a;
        return *this;
    }
    vec2<float> operator* (vec2<float> v) {
        return vec2<float>(v.x * data[0][0] + v.y * data[0][1], v.x * data[1][0] + v.y * data[1][1]);
    }
    Matrix operator+ (Matrix other) const {
        Matrix res;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                res.data[i][j] = data[i][j] + other.data[i][j];
        return res;
    }
    Matrix operator- (Matrix other) const {
        Matrix res;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                res.data[i][j] = data[i][j] - other.data[i][j];
        return res;
    }
    Matrix operator* (Matrix other) const {
        Matrix res(0,0,0,0);
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    res.data[i][j] += data[i][k] * other.data[k][j] ;
        return res;
    }
    
};
