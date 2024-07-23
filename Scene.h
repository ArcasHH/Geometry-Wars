#pragma once
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>

struct GameScene {
    std::unique_ptr<Player> player;
    bool add_enemies{ false };

    GameScene() = default;
    GameScene(const GameScene&) = delete;
    GameScene(GameScene&&) = delete;
    GameScene& operator=(const GameScene&) = delete;
    GameScene& operator=(GameScene&& Other) = delete;

    void setPlayer(Triangle tr, vec2<float> position);
    void addEnemy(Triangle tr, vec2<float> position);
    
    void act(float dt);
    void draw(BuffTy buffer)const;

private:
    std::vector<std::unique_ptr<Enemy>> EnemyBuffer;
    void enemy_act(Enemy* enemy, float dt);
    void enemy_player_collision(Enemy* enemy);
    void bullet_enemy_collision(Bullet bullet, Enemy* enemy);
    void addEnemies(int num);
};
