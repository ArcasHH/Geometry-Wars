#pragma once
#include "Actor.h"

struct Enemy : Actor {
	vec2<float> player_pos;
	int score{ 1 };
	bool kill_player{ false };
	bool is_active{ false };
	float act_timer{ TIME_TO_ACT };
	void act(float dt) override;
	//void draw(BuffTy buffer) const ;

	Enemy(Triangle tr, vec2<float> pos) : Actor(tr, pos) {}
	~Enemy() = default;
	bool is_collide(vec2<float> obj_pos, float sq_distance);
private:
	void navigate(vec2<float> player_pos);
};