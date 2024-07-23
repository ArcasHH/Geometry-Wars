#include "Scene.h"

void GameScene::act(float dt) {
    /*
    for (auto&& obj : EnemyBuffer) {
        enemy_act(obj.get(),dt);
       
    }
    */
    auto iter = EnemyBuffer.begin();
    for (auto&& obj : EnemyBuffer) {
        enemy_act(obj.get(),dt);
        if (!obj->is_alive) {
            EnemyBuffer.erase(iter);
            addEnemies(1);
            break;
        }
        iter++;
    }
    player->act(dt);

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
    //player = make_unique<Actor>(tr, myController);
}
void GameScene::addEnemy(Triangle tr, vec2<float> pos) {
    EnemyBuffer.emplace_back(std::make_unique<Enemy>(tr, pos));
    //make_unique<Actor>(tr, pos, AIController);
}

void GameScene::enemy_act(Enemy* obj, float dt) {
    if (auto* ActObj = dynamic_cast<IActable*>(obj)) {
        if (!(obj->is_alive))
            return;
        obj->player_pos = player->position; // enemy always know where player located
        ActObj->act(dt);
        enemy_player_collision(obj);
        for (int i = 0; i < AMMO_AMOUNT; ++i) { //puting a bullet in the enemy
            bullet_enemy_collision(player->ammo[i], obj);
        }
    }
}
void GameScene::enemy_player_collision(Enemy* enemy) {
    if (enemy->kill_player && (player->shock_time < FLOAT_PRECISE)) {//enemy attack
        player->health -= enemy->damage;
        player->shock_time = SHOCK_TIME;
    }
}
void GameScene::bullet_enemy_collision(Bullet bullet, Enemy* enemy) {
    if (enemy->is_collide(bullet.position, 100) && bullet.is_alive && enemy->is_active ) {
        bullet.is_alive = false;
        enemy->is_alive = false;
        player->score += enemy->score;
    }
}

void GameScene::addEnemies(int num) {
    for (int i = 0; i < num; ++i) {
        int randX = random(BOUND_WIDTH, SCREEN_WIDTH - BOUND_WIDTH);
        int randY = random(BOUND_WIDTH, SCREEN_HEIGHT - BOUND_WIDTH);
        addEnemy(Triangle(vec2<float>(15, 45), vec2<float>(0, 0), vec2<float>(30, 0), Color(200, 0, 0)), vec2<float>(randX, randY));
    }
    bool add_enemies = false;
}

