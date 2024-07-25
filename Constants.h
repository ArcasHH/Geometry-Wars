#pragma once

// Global parametrs
constexpr int BOUND_WIDTH = 32;
constexpr  float  SPEED_FADE = 0.995f; // 1 - does not fade ( x-axis ), 0 - there is no inertia motion on the x-axis  
constexpr float FLOAT_PRECISE = 1e-5f;
static_assert(SPEED_FADE < 1.f && SPEED_FADE > 0.f);


//Player parametrs
constexpr  float  MAX_SPEED = 0.8f;//player speed limit
constexpr  float  SPEED_SCALE = 1.f;//the coefficient of speed change when control(). // must not exceed the value of MAX_SPEED * 1/dt
static_assert(SPEED_SCALE < MAX_SPEED * 1000.f);
constexpr  float  SQ_COLLIDE_DISTANCE = 500.f;

constexpr int START_HEALTH = 10;
constexpr float REGENERATION_TIME = 5.f;
constexpr int START_DAMAGE = 1;
constexpr float SHOCK_TIME = 0.5f;
constexpr float PUSH_ENEMIES = 8.f; 

//Ammo parametrs
constexpr int AMMO_AMOUNT = 4;
constexpr float BULLET_SPEED = 1.f;
constexpr float AMMO_RELOAD = 0.1f;

//Enemy parametrs
constexpr  float  ENEMY_MAX_SPEED = 0.5f;
constexpr  float  ENEMY_SPEED_SCALE = 0.5f;//the coefficient of speed change. // must not exceed the value of MAX_SPEED * 1/dt
static_assert(ENEMY_SPEED_SCALE < ENEMY_MAX_SPEED * 1000.f);
constexpr  float  ENEMY_SQ_COLLIDE_DISTANCE = 200.f;







