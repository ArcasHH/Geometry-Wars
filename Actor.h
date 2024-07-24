#pragma once
#include "Interface.h"

struct Actor: public IDrawable, IActable{
	Triangle2 sprite;
	vec2<float> speed{};
	vec2<float> dir;

	int health{};
	bool is_alive{ true };
	int damage;
	Actor() = default;
	Actor(Triangle2 tr) : sprite{ tr }, dir{ 0, 1 }, damage{ 1 } {}
	virtual void act(float dt) = 0;
	virtual void draw(BuffTy buffer)const override;

	void moveBy(vec2<float> vec);
	void moveTo(vec2<float> vec);
	void turnSide(vec2<float> point);
	void control(float dt);
	bool out_of_bounds();
};
