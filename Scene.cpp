#include "Scene.h"

void GameScene::act(float dt) {
    for (auto&& obj : EnemyBuffer) {
        if (auto* ActObj = dynamic_cast<IActable*>(obj.get())) {
            ActObj->act(dt, player_pos);
        }
    }
    player->act(dt);
    player_pos = player.get()->position;
}
void GameScene::draw(BuffTy buffer) {
    for (auto&& obj : EnemyBuffer) {
        if (auto* DrObj = dynamic_cast<IDrawable*>(obj.get())) {
            DrObj->draw(buffer);
        }
    }
    player->draw(buffer);
}
void GameScene::setPlayer(Triangle tr) {
    player = std::make_unique<Player>(tr);
    player_pos = player.get()->position;
}
void GameScene::addEnemy(Triangle tr, vec2<float> pos) {
    EnemyBuffer.emplace_back(std::make_unique<Enemy>(tr));
    EnemyBuffer[EnemyBuffer.size() - 1].get()->moveTo(pos);
}