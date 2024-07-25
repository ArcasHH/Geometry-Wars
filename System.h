#pragma once
#include "Component.h"

inline Registry<cmp::Sprite, cmp::Color,
    cmp::Position, cmp::Velocity, cmp::Rotation, cmp::Direction,
    cmp::IsPlayer, cmp::IsEnemy, cmp::IsBullet,cmp::IsActive,
    cmp::CanShoot, cmp::Ammo, cmp::Health, cmp::Damage > Reg;

using BuffTy = decltype(buffer);

namespace sys {

    //act(dt)
    void act(float dt);

    void move(float dt);
    void rotate();
    void turnTowards(float dt);
    void playerRegeneration(float dt);
    void control(float dt);
    void ammoReload(float dt);

    //updating data
    void update();
    void outOfBounds();
    void bulletCollideEnemy();
    void playerCollideEnemy();
    

    //support functions
    void shoot();
    void turnTowardsCursor();
    void turnTowardsPlayer(float dt);
    void pushEnemy(vec2<float>& enemy_speed);
    void checkHealth();
    void playerHasDamage(int damage);
    void updatePlayerColor();


    //draw(buffer)
    void draw(BuffTy Buffer);

};


