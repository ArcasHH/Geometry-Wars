#pragma once
#include "Object.h"
#include "Constants.h"

struct Player{
	Triangle sprite;
	vec2<float> position;
	vec2<float> speed;
	float seg_speed;
	vec2<float> dir;
	bool is_control{ true };

	void act(float dt);
	void draw(BuffTy buffer);

    Player() = default;
	Player(Triangle tr) : sprite{ tr }, position{ tr.getCenter() }, speed{}, dir{ 0,1 }, seg_speed{0} {
		moveTo(vec2<float>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)); //move player to screen center
	}
	bool collide(Triangle& t);

private:
    void control(float dt);
    void moveBy(vec2<float> vec);
	void moveTo(vec2<float> vec);
	void rotate();
	//float get_mouse_angle();
	bool out_of_bounds(int width);
};

