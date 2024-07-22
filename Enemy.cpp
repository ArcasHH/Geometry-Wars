#include "Enemy.h"

void Enemy::act(float dt, vec2<float> player_pos) {
    kill_player = false;
    rotate(player_pos);
    
    if ((player_pos - position).sq_length() <= 255) {
        
        speed *=(-PUSHING_ENEMIES);
        moveBy(speed * MAX_SPEED_X * 100);
        kill_player = true;
    }
    navigate(player_pos);
    out_of_bounds();
    moveBy(speed);
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
    vec2<float> nav = player_pos - position;
    nav.normalize();
    if(speed.length()<= MAX_ENEMY_SPEED)
        speed += nav * SCALE_ENEMY_SPEED;
    speed *= SPEED_FADE;
}

bool Enemy::out_of_bounds() {
    vec2<float> center = sprite.getCenter();
    if (center.x <= BOUND_WIDTH) {
        speed.x = SCALE_ENEMY_SPEED * 3;
        return true;
    }
    if (center.y <= BOUND_WIDTH) {
        speed.y = SCALE_ENEMY_SPEED * 3;
        return true;
    }
    if (center.x >= SCREEN_WIDTH - BOUND_WIDTH) {
        speed.x = -SCALE_ENEMY_SPEED * 3;
        return true;
    }
    if (center.y >= SCREEN_HEIGHT - BOUND_WIDTH) {
        speed.y = -SCALE_ENEMY_SPEED * 3;
        return true;
    }
    return false;
}