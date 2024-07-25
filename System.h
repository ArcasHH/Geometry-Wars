#pragma once
#include "Component.h"

inline Registry<cmp::TriangleSprite, cmp::Color, cmp::Collision,
    cmp::Position, cmp::Velocity, cmp::Rotation, cmp::Direction,
    cmp::IsPlayer, cmp::IsEnemy, cmp::IsBullet,cmp::IsActive,
    cmp::CanShoot, cmp::Ammo, cmp::Health, cmp::Damage,
    cmp::Progress, cmp::Enemies> Reg;

using BuffTy = decltype(buffer);

namespace sys {

    //act(dt)
    void act(float dt);

    //Transform system
    void transform(float dt);
    void move(float dt);
    void rotate();
    void outOfBounds();
    void turnTowards(EntityId ent_id, vec2<float> pos);
    void moveInDir(EntityId ent_id, float dt);
    bool isEntitiesCollide(EntityId ent1_id, EntityId ent2_id);
    
    //player act
    void playerControl(float dt);
    void control(float dt); 
    void turnTowardsCursor(); 
    //Player->bullets  --- shooting
    void ammoReload(float dt);
    void shoot(float dt);

    //enemy ai
    void EnemyAI(float dt);

    
    //Bullets->enemy --- hit  enemy
    void bulletCollideEnemy();

    //Enemy->player --- enemy damage player
    void checkPlayerEnemyCollide();
    void killEnemy(EntityId enemy_id); //player get score++
    void PlayerEnemyCollide(EntityId& enemy_id);
    void pushEnemy(vec2<float>& enemy_speed);

    //health system:
    void updateHealth(float dt);

    void checkHealth();
    void Regeneration(float dt);
    void diaplayPlayerHealth();
    void getDamage(EntityId ent_id, EntityId damager_id);

    //scenario
    void updateScenario(float dt);
    void ActivatelEnemy(EntityId enemy_id);

    //render system
    void draw(BuffTy Buffer);

};


