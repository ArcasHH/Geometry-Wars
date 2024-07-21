#pragma once
#include "Object.h"
#include "Constants.h"

struct Player{
	Triangle sprite;
	vec2<float> position;
	vec2<float> speed;

	void act(float dt);
	void draw(BuffTy buffer);

    Player() = default;
    Player(Triangle tr) : sprite{ tr }, position{ tr.getCenter() }, speed{} {
		moveTo(vec2<float>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)); //move player to screen center
	}

private:
    void control(float dt);
    void moveBy(vec2<float> vec);
	void moveTo(vec2<float> vec);
};

