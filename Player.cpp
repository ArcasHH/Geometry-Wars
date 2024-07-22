#include "Player.h"




void  Player::act(float dt) {  

    rotate();
    control(dt);
    out_of_bounds();
    moveBy(speed);
    is_control = true;
}
void Player::draw(BuffTy buffer) {
    sprite.draw(buffer);
}
void Player::control(float dt) {
    //float scale_x = SPEED_SCALE_X * dt;
    //float scale_y = SPEED_SCALE_Y * dt;
    float scale_x = SPEED_SCALE_X;
    float scale_y = SPEED_SCALE_Y;
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
    }

}
void Player::moveBy(vec2<float> vec) {
    sprite.p1 += vec;
    sprite.p2 += vec;
    sprite.p3 += vec;
    position = sprite.getCenter();
}
void Player::moveTo(vec2<float> vec) {
    moveBy(vec - sprite.getCenter());
}


bool Player::out_of_bounds() {
    vec2<float> center = sprite.getCenter();
    if (center.x <= BOUND_WIDTH) {
        speed.x = SPEED_SCALE_X * 3 ;
        is_control = false;
        return true;
    }
    if (center.y <= BOUND_WIDTH) {
        speed.y = SPEED_SCALE_Y * 3;
        is_control = false;
        return true;
    }
    if (center.x >= SCREEN_WIDTH - BOUND_WIDTH) {
        speed.x = -SPEED_SCALE_X * 3;
        is_control = false;
        return true;
    }
    if (center.y >= SCREEN_HEIGHT - BOUND_WIDTH) {
        speed.y = -SPEED_SCALE_Y * 3;
        is_control = false;
        return true;
    }
    return false;
}

void Player::rotate() {
    if (get_cursor_x() > 0 && get_cursor_y() > 0 && get_cursor_x() < SCREEN_WIDTH && get_cursor_y() < SCREEN_HEIGHT) {
        vec2<float> m_pos(get_cursor_x(), get_cursor_y());
        float phi = angle_between(dir, m_pos - sprite.getCenter());
        dir = m_pos - sprite.getCenter();
        Matrix m(phi);
        vec2<float> p0 = sprite.getCenter();
        sprite.p1 = m * (sprite.p1 - p0) + p0;
        sprite.p2 = m * (sprite.p2 - p0) + p0;
        sprite.p3 = m * (sprite.p3 - p0) + p0;  
    }
    
}

