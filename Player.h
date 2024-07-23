#pragma once
#include "Actor.h"
#include "Bullet.h"


struct Player: Actor{
	std::vector<Bullet> bullets;

	bool is_control { true };
	bool can_shoot { true };

	void act(float dt) override;
	void draw(BuffTy buffer)const override;

    Player() = default;
	Player(Triangle tr, vec2<float> pos) : Actor(tr, pos) {
		health = START_HEALTH;
		damage = START_DAMAGE;
	}

private:
    void control(float dt);
	void shoot();
};

