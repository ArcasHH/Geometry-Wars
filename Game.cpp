#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include "Scene.h"
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



GameScene scene;
// initialize game data in this function
void initialize()
{
    scene.setPlayer(Triangle(vec2<float>(100, 50), vec2<float>(200, 60), vec2<float>(150, 150), Color(255, 0, 0)));
    Rectangle r;
    r.set_by_lt_rb(vec2<float>(0, 0), vec2<float>(30, SCREEN_HEIGHT), Color(0, 255, 0));
    scene.addRectangleObject(r);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();

  scene.act(dt);
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  scene.draw(buffer);
}

// free game data in this function
void finalize()
{
}

