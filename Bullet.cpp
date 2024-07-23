#include "Bullet.h"

void Bullet::act(float dt) {
	if (!is_alive)
		return;
	if (out_of_bounds()) {
		speed = vec2<float>(0, 0);
		is_alive = false;
	}
	moveBy(speed);
}
void Bullet::draw(BuffTy buffer) const {
	if (!is_alive)
		return;
	sprite.draw(buffer);
}
void Bullet::set_dir(vec2<float> direction) {
	if (!is_alive)
		return;
	turnSide(direction); // rotate sprite
	dir.normalize();
	speed = dir * BULLET_SPEED;
}