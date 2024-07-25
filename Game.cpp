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

EntityId initializeBullet() {
    auto Bullet = Reg.create();
    Reg.emplace<cmp::IsBullet>(Bullet);
    Reg.emplace<cmp::IsActive>(Bullet, false);
    Reg.emplace<cmp::TriangleSprite>(Bullet, Triangle(vec2<float>(0.f, 10.f), vec2<float>(-5.f, -5.f), vec2<float>(5.f, -5.f)));
    Reg.emplace <cmp::Color>(Bullet, cmp::Color{ 255, 255, 255 });
    Reg.emplace<cmp::Position>(Bullet, vec2<float>{});
    Reg.emplace<cmp::Velocity>(Bullet, vec2<float>{});
    Reg.emplace<cmp::Rotation>(Bullet, 0.f);
    Reg.emplace<cmp::Direction>(Bullet, vec2<float>{0.f, 1.f});

    return Bullet;
}
EntityId initializeEnemy1(vec2<float> start_position, EntityId player) {
    auto Enemy = Reg.create();
    Reg.emplace<cmp::IsEnemy>(Enemy, player, 1);
    Reg.emplace<cmp::IsActive>(Enemy, true);

    Reg.emplace<cmp::Health>(Enemy, 1, 1, false, 0.f, 0.f);
    Reg.emplace<cmp::Damage>(Enemy, 1);

    Reg.emplace<cmp::TriangleSprite>(Enemy, Triangle(vec2<float>(0.f, 30.f), vec2<float>(-15.f, -15.f), vec2<float>(15.f, -15.f)));
    Reg.emplace <cmp::Color>(Enemy, cmp::Color{ 200, 0, 0 });
    Reg.emplace<cmp::Position>(Enemy, start_position);
    Reg.emplace<cmp::Velocity>(Enemy, vec2<float>{});
    Reg.emplace<cmp::Rotation>(Enemy, 0.f);
    Reg.emplace<cmp::Direction>(Enemy, vec2<float>{0.f, 1.f});
    return Enemy;
}
EntityId initializeEnemy2(vec2<float> start_position, EntityId player) {
    auto Enemy = Reg.create();
    Reg.emplace<cmp::IsEnemy>(Enemy, player, 3);
    Reg.emplace<cmp::IsActive>(Enemy, false);

    Reg.emplace<cmp::Health>(Enemy, 5, 5, true, 0.f, REGENERATION_TIME);
    Reg.emplace<cmp::Damage>(Enemy, 2);

    Reg.emplace<cmp::TriangleSprite>(Enemy, Triangle(vec2<float>(0.f, 30.f), vec2<float>(-15.f, -15.f), vec2<float>(15.f, -15.f)));
    Reg.emplace <cmp::Color>(Enemy, cmp::Color{ 200, 0, 150 });
    Reg.emplace<cmp::Position>(Enemy, start_position);
    Reg.emplace<cmp::Velocity>(Enemy, vec2<float>{});
    Reg.emplace<cmp::Rotation>(Enemy, 0.f);
    Reg.emplace<cmp::Direction>(Enemy, vec2<float>{0.f, 1.f});
    return Enemy;
}
EntityId initializeEnemy3(vec2<float> start_position, EntityId player) {
    auto Enemy = Reg.create();
    Reg.emplace<cmp::IsEnemy>(Enemy, player, 10);
    Reg.emplace<cmp::IsActive>(Enemy, false);

    Reg.emplace<cmp::Health>(Enemy, 10, 10, true, 0.f,  REGENERATION_TIME);
    Reg.emplace<cmp::Damage>(Enemy, 5);

    Reg.emplace<cmp::TriangleSprite>(Enemy, Triangle(vec2<float>(0.f, 60.f), vec2<float>(-30.f, -30.f), vec2<float>(30.f, -30.f)));
    Reg.emplace <cmp::Color>(Enemy, cmp::Color{ 100, 0, 200 });
    Reg.emplace<cmp::Position>(Enemy, start_position);
    Reg.emplace<cmp::Velocity>(Enemy, vec2<float>{});
    Reg.emplace<cmp::Rotation>(Enemy, 0.f);
    Reg.emplace<cmp::Direction>(Enemy, vec2<float>{0.f, 1.f});
    return Enemy;
}
EntityId initializePlayer() {
    auto Player = Reg.create();
    Reg.emplace<cmp::IsPlayer>(Player, 0);
    Reg.emplace<cmp::IsActive>(Player, true);

    Reg.emplace<cmp::CanShoot>(Player, true, AMMO_RELOAD);
    Reg.emplace<cmp::Health>(Player, START_HEALTH, START_HEALTH, true, 0.f, REGENERATION_TIME);
    Reg.emplace<cmp::Damage>(Player, START_DAMAGE);

    Reg.emplace<cmp::TriangleSprite>(Player, Triangle(vec2<float>(0.f, 30.f), vec2<float>(-15.f, -15.f), vec2<float>(15.f, -15.f)));
    Reg.emplace <cmp::Color>(Player, cmp::Color{ 0, 255, 0 });
    Reg.emplace<cmp::Position>(Player, vec2<float>(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f));
    Reg.emplace<cmp::Velocity>(Player, vec2<float>{});
    Reg.emplace<cmp::Rotation>(Player, 0.f);
    Reg.emplace<cmp::Direction>(Player, vec2<float>{0.f, 1.f});

    cmp::Ammo::AmmoStorage Ammo;
    Ammo.reserve(AMMO_AMOUNT);
    for (int i = 0; i != AMMO_AMOUNT; ++i)
        Ammo.emplace_back(initializeBullet());
    Reg.emplace<cmp::Ammo>(Player, std::move(Ammo));

    return Player;
}
void initializeScenario() {
    auto Player = initializePlayer();
    auto Scenario = Reg.create();
    Reg.emplace<cmp::Progress>(Scenario, 0.f, 0);
    cmp::Enemies::EnemyStorage Enemies;
    Enemies.reserve(3*ENEMY_AMOUNT);
    for (int i = 0; i != ENEMY_AMOUNT; ++i) {
        float x_pos = random(BOUND_WIDTH, SCREEN_WIDTH - BOUND_WIDTH);
        float y_pos = random(BOUND_WIDTH, SCREEN_HEIGHT - BOUND_WIDTH);
        Enemies.emplace_back(initializeEnemy1(vec2<float>(x_pos, y_pos), Player));
    }
    for (int i = 0; i != ENEMY_AMOUNT; ++i) {
        float x_pos = random(BOUND_WIDTH, SCREEN_WIDTH - BOUND_WIDTH);
        float y_pos = random(BOUND_WIDTH, SCREEN_HEIGHT - BOUND_WIDTH);
        Enemies.emplace_back(initializeEnemy2(vec2<float>(x_pos, y_pos), Player));
    }
    for (int i = 0; i != ENEMY_AMOUNT; ++i) {
        float x_pos = random(BOUND_WIDTH, SCREEN_WIDTH - BOUND_WIDTH);
        float y_pos = random(BOUND_WIDTH, SCREEN_HEIGHT - BOUND_WIDTH);
        Enemies.emplace_back(initializeEnemy3(vec2<float>(x_pos, y_pos), Player));
    }
    Reg.emplace<cmp::Enemies>(Scenario, std::move(Enemies));
}




// initialize game data in this function
void initialize()
{ 
    initializeScenario();
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


