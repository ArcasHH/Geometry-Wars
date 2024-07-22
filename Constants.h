#pragma once

constexpr int BOUND_WIDTH = 30;

constexpr int START_HEALTH = 10;
constexpr int START_DAMAGE = 1;
constexpr float PUSHING_ENEMIES = 2.f;


constexpr  float  MAX_SPEED_X = 0.52f;//maximum possible player speed (speed limit)
constexpr  float  MAX_SPEED_Y = 0.52f;
constexpr  float  MAX_ENEMY_SPEED = 0.1f;
constexpr  float  SCALE_ENEMY_SPEED = 0.001f;
//constexpr float SQ_MAX_SPEED = 1.f;

constexpr  float  SPEED_SCALE_X = 0.001f;//the coefficient of speed change when control(). // must not exceed the value of MAX_SPEED
constexpr  float  SPEED_SCALE_Y = 0.001f;


//static_assert(SPEED_SCALE_X < MAX_SPEED_X * 1000.f);
//static_assert(SPEED_SCALE_Y < MAX_SPEED_Y * 1000.f);

constexpr  float  SPEED_FADE = 0.999f; // 1 - does not fade ( x-axis ), 0 - there is no inertia motion on the x-axis  
// SPEED_FADE comment: if you put more than 1, then everything will break
//static_assert(SPEED_FADE < 1.f);