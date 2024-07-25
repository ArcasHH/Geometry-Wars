#pragma once
#include "Component.h"

inline Registry<cmp::TriangleSprite, cmp::Color,
    cmp::Position, cmp::Velocity, cmp::Rotation, cmp::Direction,
    cmp::IsPlayer, cmp::IsEnemy, cmp::IsBullet,cmp::IsActive,
    cmp::CanShoot, cmp::Ammo, cmp::Health, cmp::Damage,
    cmp::Progress, cmp::Enemies> Reg;

using BuffTy = decltype(buffer);

namespace sys {

    //act(dt)
    void act(float dt);

    
    void turnTowards(float dt);
    //player ai
    void control(float dt);
    void turnTowardsCursor();
    //enemy ai
    void EnemyAI(float dt);
    void turnTowardsPlayer(float dt);

    //Transform system
    void transform(float dt);
    void move(float dt);
    void rotate();
    void outOfBounds();
    
    // Three type collisions of entities:
    // 
    //Player->bullets  --- shooting
    void ammoReload(float dt);
    void shoot();
    //Bullets->enemy --- hitt  enemy
    void bulletCollideEnemy();
    //Enemy->player --- enemy damage player
    void playerCollideEnemy();
    void pushEnemy(vec2<float>& enemy_speed);
    void killEnemy(EntityId enemy_id); //player get score++
    



    //health system:
    void updateHealth(float dt);
    void checkHealth();
    void Regeneration(float dt);
    void diaplayPlayerHealth();

    //scenario
    void updateScenario(float dt);
    void ActivatelEnemy(EntityId enemy_id);


    //render system
    //draw(buffer)
    void draw(BuffTy Buffer);

};


