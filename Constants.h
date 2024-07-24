#pragma once

constexpr int BOUND_WIDTH = 32;

constexpr int START_HEALTH = 10;
constexpr int START_DAMAGE = 1;
constexpr float BULLET_SPEED = 1.5f;
constexpr int AMMO_AMOUNT = 16;
constexpr float AMMO_RELOAD = 0.2f;
constexpr float SHOCK_TIME = 0.5f;
constexpr float PUSHING_ENEMIES = 6.f;
constexpr float TIME_TO_ACT = 1.f; // time before enemies start act

constexpr  float  MAX_SPEED = 1.f;//maximum possible player speed (speed limit)
constexpr  float  MAX_ENEMY_SPEED = 0.1f;
//constexpr float SQ_MAX_SPEED = 1.f;

constexpr  float  SPEED_SCALE = 0.005f;//the coefficient of speed change when control(). // must not exceed the value of MAX_SPEED



//static_assert(SPEED_SCALE_X < MAX_SPEED_X * 1000.f);
//static_assert(SPEED_SCALE_Y < MAX_SPEED_Y * 1000.f);

constexpr  float  SPEED_FADE = 0.999f; // 1 - does not fade ( x-axis ), 0 - there is no inertia motion on the x-axis  
// SPEED_FADE comment: if you put more than 1, then everything will break
//static_assert(SPEED_FADE < 1.f);

constexpr float FLOAT_PRECISE = 1e-5;