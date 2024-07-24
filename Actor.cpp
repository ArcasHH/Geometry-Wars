#include "Actor.h"
void Actor::draw(BuffTy buffer) const {
    if(is_alive)
        sprite.draw(buffer);
}

void Actor::moveBy(vec2<float> vec) {
    sprite.position += vec;
}
void Actor::moveTo(vec2<float> vec) {
    moveBy(vec - sprite.position);
}
void Actor::turnSide(vec2<float> point) { //Look in the direction of the point
    float phi = angle_between(dir, point - sprite.position);
    dir = point - sprite.position;
    sprite.rotate(phi);
}
void control(float dt) {

}
bool Actor::out_of_bounds() {
    vec2<float> center = sprite.position;
    if (center.x <= BOUND_WIDTH) {
        speed.x = SPEED_SCALE * 3;
        return true;
    }
    if (center.y <= BOUND_WIDTH) {
        speed.y = SPEED_SCALE * 3;
        return true;
    }
    if (center.x >= SCREEN_WIDTH - BOUND_WIDTH) {
        speed.x = -SPEED_SCALE * 3;
        return true;
    }
    if (center.y >= SCREEN_HEIGHT - BOUND_WIDTH) {
        speed.y = -SPEED_SCALE * 3;
        return true;
    }
    return false;
}