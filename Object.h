#pragma once
#include "Engine.h"
#include <vector>
#include <cmath>
#include "Constants.h"

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

    float sq_length() const {return x * x + y * y;}
    float length() const {return std::sqrt(sq_length());}
    void normalize() {
        float scale = 1 / length();
        x *= scale;
        y *= scale;
    }
    vec2 normalized () {
        vec2<T> vec = *this;
        vec.normalize();
        return vec;
    }

    float dot(vec2<T> a) const {return  x * a.x + y * a.y;}
    float cross(vec2<T> a) const { return x * a.y - y * a.x; }
};

struct Triangle2 final {
    vec2<float> v1, v2, v3;
    vec2<float> position;
    Color color;

    Triangle2() = default;
    Triangle2(vec2<float> pos1, vec2<float> pos2, vec2<float> pos3, Color c) :
        v1{ pos1 }, v2{ pos2 }, v3{ pos3 }, color{ c } {}
    Triangle2(vec2<float> pos1, vec2<float> pos2, vec2<float> pos3, Color c, vec2<float> pos) :
        v1{ pos1 }, v2{ pos2 }, v3{ pos3 }, color{ c }, position{ pos } {}

    Triangle2& operator=(const Triangle2& Other) = default;
    void draw(BuffTy buffer) const;

    void rotate(float phi);
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
    vec2<float> operator* (const vec2<float> &v) {
        return vec2<float>(v.x * data[0][0] + v.y * data[0][1], v.x * data[1][0] + v.y * data[1][1]);
    }
    Matrix operator+ (const Matrix& other) const {
        Matrix res;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                res.data[i][j] = data[i][j] + other.data[i][j];
        return res;
    }
    Matrix operator- (const Matrix& other) const {
        Matrix res;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                res.data[i][j] = data[i][j] - other.data[i][j];
        return res;
    }
    Matrix operator* (const Matrix& other) const {
        Matrix res(0,0,0,0);
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    res.data[i][j] += data[i][k] * other.data[k][j] ;
        return res;
    }
    
};

float angle_between(const vec2<float>& a, const vec2<float>& b);
vec2<int> get_min_max(float a, float b, float c);

float random(float a, float b);