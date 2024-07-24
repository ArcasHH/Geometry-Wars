#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include "System.h"

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

void initializePlayer() {
    auto Player = Reg.create();
    Reg.emplace<cmp::Sprite>(Player, vec2<float>(0.f, 30.f), vec2<float>(-15.f, -15.f), vec2<float>(15.f, -15.f));
    Reg.emplace<cmp::Position>(Player, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    Reg.emplace <Color>(Player, Color{ 0, 255, 0 });
    Reg.emplace<cmp::Velocity>(Player, 0.f, 0.f);
    Reg.emplace<cmp::Rotation>(Player, 0.f);
    Reg.emplace<cmp::Control>(Player, true);
    Reg.emplace<cmp::LookTowards>(Player, 0.f, 1.f);
}


void initializeEnemy(vec2<float> start_position) {
    auto Enemy = Reg.create();
    Reg.emplace<cmp::Sprite>(Enemy,vec2<float>(0.f, 30.f), vec2<float>(-15.f, -15.f), vec2<float>(15.f, -15.f));
    Reg.emplace<cmp::Position>(Enemy, start_position.x, start_position.y);
    Reg.emplace <Color>(Enemy, Color{ 200, 0, 0 });
    Reg.emplace<cmp::Velocity>(Enemy, 0.f, 0.f);
    Reg.emplace<cmp::Rotation>(Enemy, 0.f);
    Reg.emplace<cmp::Control>(Enemy, false);
    Reg.emplace<cmp::LookTowards>(Enemy, 0.f, 1.f);
}


// initialize game data in this function
void initialize()
{
    initializePlayer();
    initializeEnemy(vec2<float>(100.f, 100.f));
    initializeEnemy(vec2<float>(500.f, 700.f));
    initializeEnemy(vec2<float>(700.f, 100.f));
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();

  sys::act(dt);

}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  sys::draw(buffer);
}

// free game data in this function
void finalize()
{
    clear_buffer();
}


