#pragma once
#include "Component.h"

inline Registry<cmp::Sprite, Color,
    cmp::Position, cmp::Velocity, cmp::Rotation, cmp::Direction,
    cmp::IsPlayer, cmp::IsEnemy, cmp::IsBullet,cmp::IsActive,
    cmp::CanShoot, cmp::Ammo, cmp::Health, cmp::Damage > Reg;


namespace sys {

    //act(dt)
    void act(float dt);

    void move(float dt);
    void rotate();
    void turnTowards(float dt);


    //updating data
    void update(float dt);

    void outOfBounds();
    void control(float dt);
    void bulletCollideEnemy();
    void playerCollideEnemy();
    void ammoReload(float dt);
    
    //support functions
    void shoot();
    void turnTowardsCursor();
    void turnTowardsPlayer(float dt);
    void pushEnemy(vec2<float>& enemy_speed);


    //draw(buffer)
    void draw(BuffTy Buffer);

};


