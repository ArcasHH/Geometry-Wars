#pragma once
#include "Component.h"

inline Registry<cmp::Sprite, cmp::Position, cmp::Velocity, Color, cmp::Rotation, cmp::Direction,
    cmp::IsPlayer, cmp::IsEnemy, cmp::IsBullet, 
    cmp::IsActive > Reg;

namespace sys {

    //act(dt)
    void act(float dt);

    void move(float dt);
    void rotate(float dt);
    bool outOfBounds();
    void control(float dt);
    void turnTowards();
    void turnTowardsCursor();
    void turnTowardsPlayer();

    void shoot();
    

    //draw(buffer)
    void draw(BuffTy Buffer);
};
