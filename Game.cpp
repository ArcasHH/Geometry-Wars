#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include <cmath>

//
//  You are free to modify this file
//

using BuffTy = decltype(buffer);
union Color final{
    struct {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    };
    uint32_t data;
    Color() : data(0x000000ff) {}
    Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) {
        this->set(R, G, B, A);
    }
    void set(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) {
        r = R;
        g = G;
        b = B;
        a = A;
    }
    Color& operator=(const Color &) = default;
};

template< typename T >
struct vec2 final {
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


    T x{}, y{};
};

float seg_ratio(vec2<float>& a, vec2<float>& b){
    return  ((a.x - b.x) / (a.y - b.y));
}


class Triangle {
    vec2<float> p1, p2, p3;
    Color color;
    

    void reorganize() {   // sort points by y coord:      // p1 - min by y   // p2 - middle by y    // p3 - max by y
        if (p1.y > p3.y)
            std::swap(p1, p3);
        if (p2.y > p3.y)
            std::swap(p2, p3);
        if (p1.y > p2.y)
            std::swap(p1, p2);
    }
    void draw_horizontal_segment(BuffTy buffer, float xpos1, float xpos2, int y) const {
        int x1 = static_cast<int> (xpos1);
        int x2 = static_cast<int> (xpos2);
        if (x2 < x1)
            std::swap(x1, x2);
        for (int i = x1; i < x2; ++i)
            buffer[y][i] = color.data;
    }
  
    void draw_top(BuffTy buffer, vec2<float>&p1, vec2<float>& p2, vec2<float>& p3) const { // case when p1.y == p2.y
        vec2<float> sr(seg_ratio(p3, p1), seg_ratio(p3, p2));//wrong interpretation (This is not a vector)
        vec2<float> curr(p3.x, p3.x);                      //wrong interpretation (This is not a vector)
        int p3y = static_cast<int> (p3.y);
        int p1y = static_cast<int> (p1.y);
        for (int scanlineY = p3y; scanlineY >= p1y; --scanlineY) {
            draw_horizontal_segment(buffer, curr.x, curr.y, scanlineY);
            curr -= sr;
        }
    }
    void draw_bottom(BuffTy buffer, vec2<float>& p1, vec2<float>& p2, vec2<float>& p3) const{ // case when p3.y == p2.y
        vec2<float> sr(seg_ratio(p2, p1), seg_ratio(p3, p1));//wrong interpretation (This is not a vector)
        vec2<float> curr(p1.x,p1.x);                       //wrong interpretation (This is not a vector)
        int p2y = static_cast<int> (p2.y);
        int p1y = static_cast<int> (p1.y);
        for (int scanline = p1y; scanline <= p2y; ++scanline) {
            draw_horizontal_segment(buffer, curr.x, curr.y, scanline);
            curr += sr;
        }
    }
public:
    Triangle() = default;
    Triangle(vec2<float> pos1, vec2<float> pos2, vec2<float> pos3, Color c)
        : p1{ pos1 }, p2{ pos2 }, p3{ pos3 }, color{ c } {}

    Triangle& operator=(const Triangle& Other) = default;
    void draw(BuffTy buffer) {
        reorganize();
        vec2<float> p4((p1.x + ((p2.y - p1.y) / (p3.y - p1.y)) * (p3.x - p1.x)), p2.y);
        draw_bottom(buffer, p1, p2, p4);
        draw_top(buffer, p2, p4, p3);
    }
};



//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()


Triangle t(vec2<float>(100, 50), vec2<float>(200, 60), vec2<float>(150, 150), Color(128, 0, 64));
// initialize game data in this function
void initialize()
{

}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  t.draw(buffer);
}

// free game data in this function
void finalize()
{
}

