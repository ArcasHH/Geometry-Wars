#include "Enemy.h"

void Enemy::act(float dt) {
    kill_player = false;
    turnSide(player_pos);
    
    if ((player_pos - position).sq_length() <= 255) { 
        speed *=(-PUSHING_ENEMIES);
        moveBy(speed * PUSHING_ENEMIES * 100);
        kill_player = true;
    }
    navigate(player_pos);
    out_of_bounds();
    moveBy(speed);
}

void Enemy::navigate(vec2<float> player_pos) {
    vec2<float> nav = player_pos - position;
    nav.normalize();
    if(speed.length()<= MAX_ENEMY_SPEED)
        speed += nav * SPEED_SCALE;
    speed *= SPEED_FADE;
}
