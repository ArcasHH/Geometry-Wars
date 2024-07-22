#include "Enemy.h"

void Enemy::act(float dt, vec2<float> player_pos) {
    rotate(player_pos);
    navigate(player_pos);
}

void Enemy::draw(BuffTy buffer) {
	sprite.draw(buffer);
}

void Enemy::moveBy(vec2<float> vec) {
    sprite.p1 += vec;
    sprite.p2 += vec;
    sprite.p3 += vec;
    position = sprite.getCenter();
}
void Enemy::moveTo(vec2<float> vec) {
    moveBy(vec - sprite.getCenter());
}
void Enemy::rotate(vec2<float> player_pos) {
    float phi = angle_between(dir, player_pos - sprite.getCenter());
    dir = player_pos - sprite.getCenter();
    Matrix m(phi);
    vec2<float> p0 = sprite.getCenter();
    sprite.p1 = m * (sprite.p1 - p0) + p0;
    sprite.p2 = m * (sprite.p2 - p0) + p0;
    sprite.p3 = m * (sprite.p3 - p0) + p0;
}
void Enemy::navigate(vec2<float> player_pos) {
    speed = player_pos - position;
    speed.normalize();
    speed *= MAX_ENEMY_SPEED;
    moveBy(speed);
}