#include "Player.h"
void  Player::act(float dt) {
    control(dt);
    moveBy(speed);
}
void Player::draw(BuffTy buffer) {
    sprite.draw(buffer);
}
void Player::control(float dt) {
    float scale_x = SPEED_SCALE_X * dt;
    float scale_y = SPEED_SCALE_Y * dt;
    if (is_key_pressed('D')) {
        if (speed.x <= MAX_SPEED_X)
            speed.x += scale_x;
    }
    if (is_key_pressed('A')) {
        if (speed.x >= -MAX_SPEED_X)
            speed.x -= scale_x;
    }
    if (is_key_pressed('S')) {
        if (speed.y <= MAX_SPEED_Y)
            speed.y += scale_y;
    }
    if (is_key_pressed('W')) {
        if (speed.y >= -MAX_SPEED_Y)
            speed.y -= scale_y;
    }

    if (!is_key_pressed('A') && !is_key_pressed('D')) { // speed fading
        speed *= SPEED_FADE;
    }
}
void Player::moveBy(vec2<float> vec) {
    sprite.p1 += vec;
    sprite.p2 += vec;
    sprite.p3 += vec;
}
void Player::moveTo(vec2<float> vec) {
    moveBy(vec - sprite.getCenter());
}