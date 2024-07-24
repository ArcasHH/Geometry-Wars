#pragma once
#include "Component.h"

inline Registry<cmp::Sprite, cmp::Position, cmp::Velocity, Color, cmp::Rotation > Reg;

namespace sys {

    //act(dt)
    void move(float dt);
    void rotate(float dt);

    void act(float dt);

    //draw(buffer)
    void draw(BuffTy Buffer);
};
