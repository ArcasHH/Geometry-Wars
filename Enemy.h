#pragma once
#include "Interface.h"
#include "Constants.h"

struct Enemy : public IDrawable, IObject, IActable {
	Triangle sprite;
	vec2<float> position;
	vec2<float> speed;
	vec2<float> dir;
	int health;
	int damage;
	int score{ 0 };
	bool is_alive{true};

	void act(float dt, vec2<float> player_pos);
	void draw(BuffTy buffer);

	Enemy() = default;
	Enemy(Triangle tr) : sprite{ tr }, position{ tr.getCenter() }, speed{}, dir{ 0, 1 }, health{ 1 }, damage{ 1 } {}
	void moveBy(vec2<float> vec);
	void moveTo(vec2<float> vec);
	void rotate(vec2<float> player_pos);
	void navigate(vec2<float> player_pos);
};