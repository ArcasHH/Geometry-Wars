#pragma once
#include "Interface.h"

struct Actor: public IDrawable, IActable{
	Triangle sprite;
	vec2<float> position;
	vec2<float> speed{};
	vec2<float> dir;

	int health{};
	bool is_alive{ true };
	int damage;
	Actor() = default;
	Actor(Triangle tr, vec2<float> pos) : sprite{ tr }, position{ pos }, dir{ 0, 1 }, damage{ 1 } {
		moveTo(pos);
	}
	virtual void act(float dt) = 0;
	virtual void draw(BuffTy buffer)const override;

	void moveBy(vec2<float> vec);
	void moveTo(vec2<float> vec);
	void turnSide(vec2<float> point);
	void control(float dt);
	bool out_of_bounds();
};
