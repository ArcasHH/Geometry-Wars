#pragma once
#include "Component.h"

inline Registry<cmp::Sprite, cmp::Position, cmp::Velocity, Color, cmp::Rotation, cmp::Control, cmp::LookTowards > Reg;

namespace sys {

    //act(dt)
    void act(float dt);

    void move(float dt);
    void rotate(float dt);
    bool outOfBounds();
    void control(float dt);
    void turnTowardsPoint();
    void LookAtPoint(cmp::Position);

    //draw(buffer)
    void draw(BuffTy Buffer);
};
