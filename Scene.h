#pragma once
#include "Object.h"
#include "Player.h"
#include <memory>
struct GameScene {
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Triangle>> Storage;

    GameScene() = default;
    GameScene(const GameScene&) = delete;
    GameScene(GameScene&&) = delete;
    GameScene& operator=(const GameScene&) = delete;
    GameScene& operator=(GameScene&& Other) = delete;

    void setPlayer(Triangle tr) {
        player = std::make_unique<Player>(tr);
    }
    void addTriangleObject(Triangle tr) {
        Storage.emplace_back(std::make_unique<Triangle>(tr));
    }
    void addRectangleObject(Rectangle rect) {
        Storage.emplace_back(std::make_unique<Triangle>(rect.t1));
        Storage.emplace_back(std::make_unique<Triangle>(rect.t2));
    }
    
    void act(float dt) {
        player->act(dt);
    }
    void draw(BuffTy buffer) {
        for (auto&& obj : Storage) {
            obj->draw(buffer);
        }
        player->draw(buffer);
    }

};
