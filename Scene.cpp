#include "Scene.h"

void GameScene::act(float dt) {
    
    for (auto&& obj : EnemyBuffer) {
        if (auto* ActObj = dynamic_cast<IActable*>(obj.get())) {
            ActObj->act(dt, player_pos);
            if (obj->kill_player) {
                player->health -= obj->damage;
                player->sprite.color = Color(255, 0, 0);
            }
        }
    }
    player->act(dt);
    player_pos = player.get()->position;
    if (player.get()->health <= 0) {
        schedule_quit_game();
    }
    
}
void GameScene::draw(BuffTy buffer) {
    for (auto&& obj : EnemyBuffer) {
        if (auto* DrObj = dynamic_cast<IDrawable*>(obj.get())) {
            DrObj->draw(buffer);
        }
    }
    player->draw(buffer);
    player.get()->sprite.color = Color(255, 255, 255);
    
}

void GameScene::setPlayer(Triangle tr) {
    player = std::make_unique<Player>(tr);
    player_pos = player.get()->position;


    //player = make_unique<Actor>(tr, myController);

}
void GameScene::addEnemy(Triangle tr, vec2<float> pos) {
    auto &&Emplaced = EnemyBuffer.emplace_back(std::make_unique<Enemy>(tr));
    Emplaced->moveTo(pos);

    //make_unique<Actor>(tr, pos, AIController);
}