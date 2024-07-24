#pragma once
#include "Actor.h"

struct Bullet:Actor{
	
	bool is_hit{ false };

	void act(float dt) override;
	void draw(BuffTy buffer)const override;

	Bullet() = default;
	Bullet(Triangle2 tr, vec2<float> direction) : Actor(tr) {
		is_alive = false;
		dir = direction;
		speed = dir.normalized() * BULLET_SPEED;
	}
	void set_dir(vec2<float> direction);
};

