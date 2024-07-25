#pragma once

// Global parametrs
constexpr int BOUND_WIDTH = 32;
constexpr  float  SPEED_FADE = 0.999f; // 1 - does not fade ( x-axis ), 0 - there is no inertia motion on the x-axis  
constexpr float FLOAT_PRECISE = 1e-5f;
static_assert(SPEED_FADE < 1.f && SPEED_FADE > 0.f);
//constexpr float GAME_SPEED = 300.f;

//Player parametrs
constexpr  float  MAX_SPEED = 3000.f;//player speed limit
constexpr  float  SPEED_SCALE = 400.f;//the coefficient of speed change when control(). // must not exceed the value of MAX_SPEED * 1/dt
static_assert(SPEED_SCALE < MAX_SPEED * 1000.f);

constexpr int START_HEALTH = 10;
constexpr float REGENERATION_TIME = 5.f;
constexpr int START_DAMAGE = 1;
constexpr float SHOCK_TIME = 0.5f;
constexpr float PUSH_ENEMIES = 2.f; 

//Ammo parametrs
constexpr int AMMO_AMOUNT = 16;
constexpr float BULLET_SPEED = 900.f;
constexpr float AMMO_RELOAD = 0.1f;

//Enemy parametrs
constexpr int ENEMY_AMOUNT = 2;
constexpr  float  ENEMY_MAX_SPEED = 800.f;
constexpr  float  ENEMY_SPEED_SCALE = 100.f;//the coefficient of speed change. // must not exceed the value of MAX_SPEED * 1/dt
static_assert(ENEMY_SPEED_SCALE < ENEMY_MAX_SPEED * 1000.f);







