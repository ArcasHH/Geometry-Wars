#pragma once
constexpr  float  MAX_SPEED_X = 0.52f;//maximum possible player speed (speed limit)
constexpr  float  MAX_SPEED_Y = 0.52f;

constexpr  float  SPEED_SCALE_X = 3.f;//the coefficient of speed change when control(). // must not exceed the value of MAX_SPEED
constexpr  float  SPEED_SCALE_Y = 3.f;
//static_assert(SPEED_SCALE_X < MAX_SPEED_X * 1000.f);
//static_assert(SPEED_SCALE_Y < MAX_SPEED_Y * 1000.f);

constexpr  float  SPEED_FADE = 0.995f; // 1 - does not fade ( x-axis ), 0 - there is no inertia motion on the x-axis  
// SPEED_FADE comment: if you put more than 1, then everything will break
//static_assert(SPEED_FADE < 1.f);