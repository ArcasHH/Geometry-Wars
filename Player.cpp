#include "Player.h"


void  Player::act(float dt)  {
    can_shoot = true;
    if (get_cursor_x() > 0 && get_cursor_y() > 0 && get_cursor_x() < SCREEN_WIDTH && get_cursor_y() < SCREEN_HEIGHT)
        turnSide(vec2<float>(get_cursor_x(), get_cursor_y()));
    control(dt);
    out_of_bounds();
    moveBy(speed);
    is_control = true;
    for (auto&& bullet : bullets) {
        bullet.act(dt);
    }
}
void Player::draw(BuffTy buffer) const{
    sprite.draw(buffer);
    for (auto&& bullet : bullets) {
        bullet.draw(buffer);
    }
}
void Player::control(float dt) {
    //float scale_x = SPEED_SCALE_X * dt;
    //float scale_y = SPEED_SCALE_Y * dt;
    float scale_x = SPEED_SCALE;
    float scale_y = SPEED_SCALE;
    if (is_control) {
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
        if (!is_key_pressed('A') && !is_key_pressed('D') ) { // speed fading
            speed.x *= SPEED_FADE;
        }
        if (!is_key_pressed('W') && !is_key_pressed('S')) { // speed fading
            speed.y *= SPEED_FADE;
        }
        if (is_mouse_button_pressed(0) && can_shoot) { // speed fading
            shoot();
            can_shoot = false;//need to add timer
        }
    }

}

void Player::shoot() {
     vec2<float> bullet_speed = dir;
     bullet_speed.normalize();
     bullet_speed *= BULLET_SPEED;
     Bullet bullet(Triangle(vec2<float>(2.5, 5), vec2<float>(0, 0), vec2<float>(5, 0), Color(255, 255, 255)), position, bullet_speed);
     bullets.push_back(bullet);
     can_shoot = false;

}

