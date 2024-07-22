#pragma once
#include "Interface.h"

struct Bullet: public IDrawable, IActable{
	vec2<float> speed;
	vec2<float> position;
	int damage;
	bool is_hit{ false };

	void draw(BuffTy buffer);
	void act(float dt, vec2<float> player_pos);
	void MoveBy(vec2<float> vec);

	Bullet() = default;
	Bullet(Triangle tr, vec2<float> pos, vec2<float> start_speed) : speed{ start_speed }, position{ pos }, damage{ 1 } {}
};

