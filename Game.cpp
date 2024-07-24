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


// initialize game data in this function
void initialize()
{
    auto Sect = Reg.create();

    Reg.emplace<cmp::Sprite>(Sect, vec2<float>(0.f, 30.f), vec2<float>(-15.f, -15.f), vec2<float>(15.f, -15.f));
    Reg.emplace<cmp::Position>(Sect, 100.f, 100.f);
    Reg.emplace <Color>(Sect, Color{0, 255, 0});
    Reg.emplace<cmp::Velocity>(Sect, 0.1f, 0.1f);
    Reg.emplace<cmp::Rotation>(Sect, 0.001f);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();

  sys::move(dt);
  sys::rotate(dt);

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
}

