#include "Player.h"


float scalar_product(vec2<float> a, vec2<float> b) {
    return  b.x * a.x + b.y * a.y;
}
float angle_between(vec2<float> a, vec2<float> b) {
    if ((a.sq_length() * b.sq_length()) != 0) {
        float dot = scalar_product(a, b) / (b.length() * a.length());
        if (dot > 1) dot = 1.f;
        else if (dot < -1) dot = -1.f;
        if (a.x * b.y - a.y * b.x > 0) {       
            return std::acos(dot);
        }   
        else {   
            return -(std::acos(dot));
        }
    }

    else return 0.f;
}
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

    rotate();
    control(dt);
    out_of_bounds(30);
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

bool Player::out_of_bounds(int bound_width) {
    vec2<float> center = sprite.getCenter();
    if (center.x <= bound_width ) {
        speed.x = SPEED_SCALE_X*2 ;
        moveBy(vec2<float>(MAX_SPEED_X *10, 0));
        is_control = false;
        return true;
    }
    if (center.y <= bound_width ) {
        speed.y = SPEED_SCALE_Y * 2;
        moveBy(vec2<float>(0, MAX_SPEED_Y* 10));

        is_control = false;
        return true;
    }
    if (center.x >= SCREEN_WIDTH - bound_width ) {
        speed.x = -SPEED_SCALE_X * 2;
        moveBy(vec2<float>( - MAX_SPEED_X* 10, 0));

        is_control = false;
        return true;
    }
    if (center.y >= SCREEN_HEIGHT - bound_width ) {
        speed.y = -SPEED_SCALE_Y * 2;
        moveBy(vec2<float>(0,  - MAX_SPEED_Y* 10));

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

