#pragma once
#include "Constants.h"
#include "Interface.h"
#include "Bullet.h"


struct Player: public IDrawable, IObject{
	Triangle sprite;
	std::vector<Bullet> bullets;
	vec2<float> position;
	vec2<float> speed;
	vec2<float> dir; //need for turning towards the cursor
	bool is_control { true };

	int health;
	bool is_alive { true };
	bool can_shoot { true };
	int damage;

	void act(float dt);
	void draw(BuffTy buffer);

    Player() = default;
	Player(Triangle tr) : sprite{ tr }, position{ tr.getCenter() }, speed{}, dir{ 0, 1 }, health{ START_HEALTH }, damage{ START_DAMAGE } {
		moveTo(vec2<float>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)); //move player to screen center
	}
	//bool collide(Triangle& t);

private:
    void control(float dt);
    void moveBy(vec2<float> vec);
	void moveTo(vec2<float> vec);
	void rotate();
	bool out_of_bounds();
	void shoot();
};

