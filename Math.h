#pragma once
#include "Engine.h"
#include <vector>
#include <cmath>
#include "Constants.h"



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

    float sq_length() const { return x * x + y * y; }
    float length() const { return std::sqrt(sq_length()); }
    void normalize() {
        float scale = 1 / length();
        x *= scale;
        y *= scale;
    }
    vec2 normalized() {
        vec2<T> vec = *this;
        vec.normalize();
        return vec;
    }

    float dot(vec2<T> a) const { return  x * a.x + y * a.y; }
    float cross(vec2<T> a) const { return x * a.y - y * a.x; }
};


struct Matrix {
    float data[2][2];
    Matrix() = default;
    Matrix(float phi) : data{ std::cos(phi), -std::sin(phi) ,std::sin(phi) ,std::cos(phi) } {}
    Matrix(float x11, float x12, float x21, float x22) : data{ x11,x12,x21,x22 } { }

    Matrix operator* (float a) {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                data[i][j] *= a;
        return *this;
    }
    vec2<float> operator* (const vec2<float>& v) {
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
        Matrix res(0, 0, 0, 0);
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                    res.data[i][j] += data[i][k] * other.data[k][j];
        return res;
    }
};

float angle_between(const vec2<float>& a, const vec2<float>& b);
vec2<int> get_min_max(float a, float b, float c);
float random(float a, float b);

void rotateSprite(float phi, vec2<float>& v1, vec2<float>& v2, vec2<float>& v3);
void rotateVector(float phi, vec2<float>& v);
bool point_in_triangle(const vec2<int>& vec, vec2<float> p1, vec2<float> p2, vec2<float> p3);
