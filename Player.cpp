#include "Player.h"



float point_segment(vec2<float> c, vec2<float> a, vec2<float> b) {
    return (c.x * (a.y - b.y) + c.y * (b.x - a.x) + a.x * b.y - a.y * b.x);
}
bool segment_intersect(vec2<float> a, vec2<float> b, vec2<float> c, vec2<float> d) { // ab intersect cd?
    if ((point_segment(c, a, b) * point_segment(d, a, b) <= 0) &&
        (point_segment(a, c, d) * point_segment(b, c, d) <= 0))
        return true;
    return false;
}

void  Player::act(float dt) {
    control(dt);
    out_of_bounds();
    moveBy(speed);
}
void Player::draw(BuffTy buffer) {
    sprite.draw(buffer);
}
void Player::control(float dt) {
    //float scale_x = SPEED_SCALE_X * dt;
    //float scale_y = SPEED_SCALE_Y * dt;
    float scale_x = SPEED_SCALE_X;
    float scale_y = SPEED_SCALE_Y;

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

bool Player::collide(Triangle& t) {
    if (segment_intersect(sprite.p1, sprite.p2, t.p1, t.p2) || segment_intersect(sprite.p1, sprite.p2, t.p2, t.p3) || segment_intersect(sprite.p1, sprite.p2, t.p1, t.p3) ||
        segment_intersect(sprite.p2, sprite.p3, t.p1, t.p2) || segment_intersect(sprite.p2, sprite.p3, t.p2, t.p3) || segment_intersect(sprite.p2, sprite.p3, t.p1, t.p3) ||
        segment_intersect(sprite.p1, sprite.p3, t.p1, t.p2) || segment_intersect(sprite.p1, sprite.p3, t.p2, t.p3) || segment_intersect(sprite.p1, sprite.p3, t.p1, t.p3)) {
        speed = vec2 < float>(0, 0);
        return true;
    }
        
    return false;
}

bool Player::out_of_bounds() {
    vec2<float> v1 = sprite.p1 + speed;
    vec2<float> v2 = sprite.p2 + speed;
    vec2<float> v3 = sprite.p3 + speed;
    if (v1.x <= 1 || v2.x <= 1 || v3.x <= 1) {
        speed.x = SPEED_SCALE_X;
        return true;
    }
    if (v1.y <= 1 || v2.y <= 1 || v3.y <= 1) {
        speed.y = SPEED_SCALE_Y;
        return true;
    }
    if (v1.x >= SCREEN_WIDTH-1 || v2.x >= SCREEN_WIDTH-1 || v3.x >= SCREEN_WIDTH-1) {
        speed.x = -SPEED_SCALE_X;
        return true;
    }
    if (v1.y >= SCREEN_HEIGHT-1 || v2.y >= SCREEN_HEIGHT-1 || v3.y >= SCREEN_HEIGHT-1) {
        speed.y = -SPEED_SCALE_Y;
        return true;
    }
    return false;
}

