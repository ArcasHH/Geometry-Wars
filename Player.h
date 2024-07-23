#pragma once
#include "Actor.h"
#include "Bullet.h"


struct Player: Actor{
	Bullet ammo[AMMO_AMOUNT];

	bool is_control { true };
	bool can_shoot { true };

	float ammo_reload{ 0 };
	float shock_time{ 0 };

	int score{ 0 };
	void act(float dt) override;
	void draw(BuffTy buffer)const override;

    Player() = default;
	Player(Triangle tr, vec2<float> pos) : Actor(tr, pos) {
		health = START_HEALTH;
		damage = START_DAMAGE;

		Bullet bullet(Triangle(vec2<float>(5, 20), vec2<float>(0, 0), vec2<float>(10, 0), Color(255, 255, 255)), position, dir);
		for (int i = 0; i < AMMO_AMOUNT; ++i) {
			ammo[i] = bullet;
		}
	}

private:
    void control(float dt);
	void shoot();
	void ammoReload(float dt);
};

