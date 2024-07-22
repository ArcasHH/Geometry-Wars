#pragma once
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>

struct GameScene {
    vec2<float> player_pos;
    std::unique_ptr<Player> player;


    GameScene() = default;
    GameScene(const GameScene&) = delete;
    GameScene(GameScene&&) = delete;
    GameScene& operator=(const GameScene&) = delete;
    GameScene& operator=(GameScene&& Other) = delete;

    void setPlayer(Triangle tr);
    void addEnemy(Triangle tr, vec2<float> position);
    
    void act(float dt);
    void draw(BuffTy buffer);

private:
    std::vector<std::unique_ptr<Enemy>> EnemyBuffer;

};
