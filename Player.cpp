#include "Player.h"
#include<iostream>

void  Player::act(float dt)  {
    if(shock_time>0)
        shock_time -= dt;
    if (get_cursor_x() > 0 && get_cursor_y() > 0 && get_cursor_x() < SCREEN_WIDTH && get_cursor_y() < SCREEN_HEIGHT)
        turnSide(vec2<float>(static_cast<float>(get_cursor_x()), static_cast<float>(get_cursor_y())));
    control(dt);
    ammoReload(dt);
    out_of_bounds();
    moveBy(speed);
    is_control = true;
    for (auto&& bullet : ammo) {
        bullet.act(dt);
    }
    if (health <= 0)
        schedule_quit_game(); ///die_screen

}
void Player::draw(BuffTy buffer) const{
    sprite.draw(buffer);
    for (auto&& bullet : ammo) {
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
            if (speed.x <= MAX_SPEED)
                speed.x += scale_x;
        }
        if (is_key_pressed('A')) {
            if (speed.x >= -MAX_SPEED)
                speed.x -= scale_x;
        }
        if (is_key_pressed('S')) {
            if (speed.y <= MAX_SPEED)
                speed.y += scale_y;
        }
        if (is_key_pressed('W')) {
            if (speed.y >= -MAX_SPEED)
                speed.y -= scale_y;
        }
        if (!is_key_pressed('A') && !is_key_pressed('D') ) { // speed fading
            speed.x *= SPEED_FADE;
        }
        if (!is_key_pressed('W') && !is_key_pressed('S')) { // speed fading
            speed.y *= SPEED_FADE;
        }
        if (is_mouse_button_pressed(0) && can_shoot) { 
            shoot();       
        }
    }
    
    
        
}

void Player::shoot() {
    for (int i = 0; i < AMMO_AMOUNT; ++i) {
        if (!ammo[i].is_alive) {
            ammo[i].is_alive = true;
            ammo[i].moveTo(position);
            ammo[i].set_dir(vec2<float>(static_cast<float>(get_cursor_x()), static_cast<float>(get_cursor_y())));        
            break;
        }
     }
    ammo_reload = 0.f;
    can_shoot = false;

}
void Player::ammoReload(float dt) {
    ammo_reload += dt;
    if (ammo_reload >= AMMO_RELOAD && !can_shoot) {
        ammo_reload = 0.f;
        can_shoot = true;
    }
}


