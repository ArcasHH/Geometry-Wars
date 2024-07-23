#include "Scene.h"

void GameScene::act(float dt) {
    
    for (auto&& obj : EnemyBuffer) {
        if (auto* ActObj = dynamic_cast<IActable*>(obj.get())) {
            obj->player_pos = player->position; // enemy always know where player located
            ActObj->act(dt);
            if (obj->kill_player) {
                player->health -= obj->damage;
            }
        }
    }
    player->act(dt); 
    if (player->health <= 0) {
        schedule_quit_game();
    }
    
}
void GameScene::draw(BuffTy buffer) const{
    for (auto&& obj : EnemyBuffer) {
        if (auto* DrObj = dynamic_cast<IDrawable*>(obj.get())) {
            DrObj->draw(buffer);
        }
    }
    player->draw(buffer);    
}

void GameScene::setPlayer(Triangle tr, vec2<float> pos) {
    player = std::make_unique<Player>(tr, pos);
    player_pos = player->position;


    //player = make_unique<Actor>(tr, myController);

}
void GameScene::addEnemy(Triangle tr, vec2<float> pos) {
    EnemyBuffer.emplace_back(std::make_unique<Enemy>(tr, pos));

    //make_unique<Actor>(tr, pos, AIController);
}