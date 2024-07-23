#pragma once
#include "Actor.h"

struct Enemy : Actor {
	vec2<float> player_pos;
	int score{ 0 };
	bool kill_player{ false };

	void act(float dt) override;
	//void draw(BuffTy buffer) const ;

	Enemy(Triangle tr, vec2<float> pos) : Actor(tr, pos) {}

	bool is_collide(vec2<float> obj_pos, float sq_distance);
private:
	void navigate(vec2<float> player_pos);
};