#pragma once
#include "Component.h"

inline Registry<cmp::Sprite, cmp::Position, cmp::Velocity, Color, cmp::Rotation, cmp::Direction,
    cmp::IsPlayer, cmp::IsEnemy, cmp::IsBullet,
    cmp::IsActive, cmp::CanShoot, cmp::Ammo > Reg;


namespace sys {

    //act(dt)
    void act(float dt);

    void move(float dt);
    void rotate(float dt);
    void outOfBounds();
    void control(float dt);
    void turnTowards();
    void turnTowardsCursor();
    void turnTowardsPlayer();

    void shoot();
    void bulletOutOfBounds();
    void bulletCollideEnemy();
    

    //draw(buffer)
    void draw(BuffTy Buffer);
};

EntityId getPlayer();
void ammoReload(float dt);
