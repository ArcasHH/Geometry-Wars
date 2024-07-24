#include "Enemy.h"

void Enemy::act(float dt) {
    if (!is_alive)
        this->~Enemy();
    if (act_timer > 0) {
        sprite.color = Color(100, 0, 0);
        act_timer -= dt;
    }
        
    else {
        is_active = true;
        sprite.color = Color(200, 0, 0);
        kill_player = false;
        turnSide(player_pos);
        navigate(player_pos);
        if ((player_pos - sprite.position).sq_length() <= 255) {
            speed *= (-PUSHING_ENEMIES);
            moveBy(speed * PUSHING_ENEMIES * 25); // here coef to avoid  bumping player into  the enemy in next frame
            kill_player = true;
        }

        out_of_bounds();
        moveBy(speed);
    }
}

void Enemy::navigate(vec2<float> player_pos) {
    if(speed.length()<= MAX_ENEMY_SPEED)
        speed += (player_pos - sprite.position).normalized() * SPEED_SCALE;
    speed *= SPEED_FADE;
}

bool Enemy::is_collide(vec2<float> obj_pos, float sq_distance) {
    if ((sprite.position - obj_pos).sq_length() <= sq_distance)
        return true;
    return false;
}
