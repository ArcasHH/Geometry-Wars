#include "System.h"


//act(dt)
void sys::act(float dt) { 

    transform(dt);

    turnTowards(dt);

    

    control(dt);
    ammoReload(dt);

    updateHealth(dt);
    updateScenario(dt);

    playerCollideEnemy();
    bulletCollideEnemy();
}

//Transform system: needs componets as position, velocity and rotation (maybe union velocity and position to one component tranformComponent?)
void sys::transform(float dt) {
    rotate();
    move(dt); 
    outOfBounds();
}
void sys::move(float dt) {
    auto& View = Reg.view<cmp::Velocity>();
    for (auto&& [Ent, Vel] : View) {
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);
        if (!CPos || !CActive->is_active)  
            continue;
        CPos->position += Vel.velocity;
        //for all entities that are not bullets, the velocity fades
        auto CBullet = Reg.findComponentOrNull<cmp::IsBullet>(Ent);
        if (!CBullet)
            Vel.velocity *= SPEED_FADE;
    }
}
void sys::rotate() {
    auto& View = Reg.view<cmp::Rotation>();
    for (auto&& [Ent, Angle] : View) {
        auto CTr = Reg.findComponentOrNull<cmp::TriangleSprite>(Ent);
        if (!CTr)  continue;
        CTr->sprite.rotate(Angle.phi);
        Angle.phi = 0.f;
    }
}
void sys::outOfBounds() {
    auto& View = Reg.view<cmp::Position>();
    for (auto&& [Ent, Pos] : View) {
        bool is_out = false;

        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(Ent);

        if (!CVel || !(CActive->is_active))
            continue;
        if (Pos.position.x < BOUND_WIDTH) {
            CVel->velocity.x = 0;
            Pos.position.x = BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.position.y < BOUND_WIDTH) {
            CVel->velocity.y = 0;
            Pos.position.y = BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.position.x > SCREEN_WIDTH - BOUND_WIDTH) {
            CVel->velocity.x = 0;
            Pos.position.x = SCREEN_WIDTH - BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.position.y > SCREEN_HEIGHT - BOUND_WIDTH) {
            CVel->velocity.y = 0;
            Pos.position.y = SCREEN_HEIGHT - BOUND_WIDTH;
            is_out = true;
        }
        //if the bullet has reached the border, then it returns to the ammo storage
        auto CBullet = Reg.findComponentOrNull<cmp::IsBullet>(Ent);
        if (CBullet && is_out) { // until the bullet goes beyond the border, it cannot be used
            CActive->is_active = false;
        }
    }
}



void sys::killEnemy(EntityId enemy_id) {
    auto PlayerEnt = Reg.getPlayer();
    auto CScore = Reg.findComponentOrNull<cmp::IsPlayer>(PlayerEnt);
    auto CEnemy = Reg.findComponentOrNull<cmp::IsEnemy>(enemy_id);
    CScore->curr_score += CEnemy->cost;

}

void sys::control(float dt) {
    auto& View = Reg.view<cmp::IsPlayer>();
    for (auto&& [Ent, _] : View) {  
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(Ent);
        if (!CVel)
            continue;
        float scale = SPEED_SCALE * dt;
        if (is_key_pressed('D')) {
            if (CVel->velocity.x <= MAX_SPEED)
                CVel->velocity.x += scale;
        }
        if (is_key_pressed('A')) {
            if (CVel->velocity.x >= -MAX_SPEED)
                CVel->velocity.x -= scale;
        }
        if (is_key_pressed('S')) {
            if (CVel->velocity.y <= MAX_SPEED)
                CVel->velocity.y += scale;
        }
        if (is_key_pressed('W')) {
            if (CVel->velocity.y >= -MAX_SPEED)
                CVel->velocity.y -= scale;
        }
        if (is_mouse_button_pressed(0)) {
            auto CShoot = Reg.findComponentOrNull<cmp::CanShoot>(Ent);
            if (CShoot->can_shoot) {
                sys::shoot();
                CShoot->reload_timer = AMMO_RELOAD;
                CShoot->can_shoot = false;
            }   
        }
    }
}
void sys::shoot() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(PlayerEnt);
    auto CPlayerDir = Reg.findComponentOrNull<cmp::Direction>(PlayerEnt);

    auto CAmmo = Reg.findComponentOrNull<cmp::Ammo>(PlayerEnt);

    for (int i = 0; i < AMMO_AMOUNT; ++i) {
        EntityId BulletEnt = CAmmo->ammo_store[i];
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(BulletEnt);
        if (CActive->is_active)
            continue;
        CActive->is_active = true;
        auto CPos = Reg.findComponentOrNull<cmp::Position>(BulletEnt);
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(BulletEnt);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(BulletEnt);
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(BulletEnt);
        if (!CPos || !CVel || !CDir || !CRot )
            continue;
        //turn towards the direction of the player
        CPos->position = CPlayerPos->position;
        CRot->phi = angle_between( CDir->direction,  CPlayerDir->direction);
        CDir->direction = CPlayerDir->direction;
        //Speed Update
        CVel->velocity =  CDir->direction * BULLET_SPEED;
        break;
    }
}

void sys::bulletCollideEnemy() {
    auto PlayerEnt = Reg.getPlayer();
    auto CAmmo = Reg.findComponentOrNull<cmp::Ammo>(PlayerEnt);
    for (int i = 0; i < AMMO_AMOUNT; ++i) {
        EntityId BulletEnt = CAmmo->ammo_store[i];
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(BulletEnt);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(BulletEnt);
        auto CColl = Reg.findComponentOrNull<cmp::Collision>(BulletEnt);

        if (!CActive->is_active || !CActive || !CPos || !CColl)
            continue;

        auto& View = Reg.view<cmp::IsEnemy>();
        for (auto&& [EnemyEnt, _] : View) {
            auto CEnemyPos = Reg.findComponentOrNull<cmp::Position>(EnemyEnt);
            auto CEnemyActive = Reg.findComponentOrNull<cmp::IsActive>(EnemyEnt);
            auto CEnemyColl = Reg.findComponentOrNull<cmp::Collision>(EnemyEnt);

            if (!CEnemyPos || !(CEnemyActive->is_active) || !CEnemyColl)
                continue;
            if ((CPos->position - CEnemyPos->position).sq_length() <= 
                (CColl->collide_distance + CEnemyColl->collide_distance)* (CColl->collide_distance + CEnemyColl->collide_distance)  ) {
                
                CActive->is_active = false;
                auto CPlayerDamage = Reg.findComponentOrNull<cmp::Damage>(PlayerEnt);
                auto CEnemyHealth = Reg.findComponentOrNull<cmp::Health>(EnemyEnt);
                if (!CPlayerDamage || !CEnemyHealth)
                    continue;
                CEnemyHealth->curr_health -= CPlayerDamage->damage;
            }
        }
    }
}

void sys::playerCollideEnemy() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(PlayerEnt);
    auto CPlayerColl = Reg.findComponentOrNull<cmp::Collision>(PlayerEnt);

    auto& View = Reg.view<cmp::IsEnemy>();
    for (auto&& [EnemyEnt, _] : View) {
        auto CEnemyPos = Reg.findComponentOrNull<cmp::Position>(EnemyEnt);
        auto CEnemyColl = Reg.findComponentOrNull<cmp::Collision>(EnemyEnt);
        auto CEnemyVel = Reg.findComponentOrNull<cmp::Velocity>(EnemyEnt);

        auto CEnemyActive = Reg.findComponentOrNull<cmp::IsActive>(EnemyEnt);

        if (!CEnemyActive->is_active || !CEnemyPos || !CEnemyColl)
            continue;
        if ((CPlayerPos->position - CEnemyPos->position).sq_length() <= 
            (CPlayerColl->collide_distance+CEnemyColl->collide_distance)* (CPlayerColl->collide_distance + CEnemyColl->collide_distance)) {
            pushEnemy(CEnemyVel->velocity);
            auto CPlayerHealth = Reg.findComponentOrNull<cmp::Health>(PlayerEnt);
            auto CEnemyDamage = Reg.findComponentOrNull<cmp::Damage>(EnemyEnt);
            if (!CPlayerHealth || !CEnemyDamage)
                continue;
            CPlayerHealth->curr_health -= CEnemyDamage->damage;
        }
    }
}

void sys::pushEnemy(vec2<float>& enemy_speed) {
    enemy_speed *= (-PUSH_ENEMIES);
    if (enemy_speed.length() > 10 * ENEMY_MAX_SPEED)
        enemy_speed = enemy_speed.normalized() * 10 * ENEMY_MAX_SPEED;
}

void sys::ammoReload(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto CShoot = Reg.findComponentOrNull<cmp::CanShoot>(PlayerEnt);
    if (CShoot->reload_timer < FLOAT_PRECISE) {
        CShoot->can_shoot = true;
    }
    else {
        CShoot->reload_timer -=dt;
    }
}

void sys::turnTowardsCursor() {
    auto& View = Reg.view<cmp::IsPlayer>();
    for (auto&& [Ent, Player] : View) {
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(Ent);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(Ent);
        if (!CRot || !CPos || !CDir)
            continue;
        vec2<float> NewDir(get_cursor_x() - CPos->position.x, get_cursor_y() - CPos->position.y);
        CRot->phi = angle_between(CDir->direction, NewDir);
        rotateVector(CRot->phi, CDir->direction);
    }
}
void sys::turnTowardsPlayer(float dt) {
    auto& View = Reg.view<cmp::IsEnemy>();
    for (auto&& [EnemyEnt, Enemy] : View) {
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(EnemyEnt);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(EnemyEnt);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(EnemyEnt);
        auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(Enemy.player_id);

        if (!CRot || !CPos || !CDir || !CPlayerPos)
            continue;
        CRot->phi = angle_between(CDir->direction, CPlayerPos->position - CPos->position);
        rotateVector(CRot->phi, CDir->direction);
        //Speed Update
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(EnemyEnt);
        if (CVel && (CVel->velocity).length() < ENEMY_MAX_SPEED) {
            CVel->velocity += CDir->direction * ENEMY_SPEED_SCALE * dt;
        }
    }
}
void sys::turnTowards(float dt) {
    sys::turnTowardsCursor();
    sys::turnTowardsPlayer(dt);
}



// Health system
void sys::updateHealth(float dt) {
    checkHealth();
    Regeneration(dt);
    diaplayPlayerHealth();
}
void sys::checkHealth() {
    auto& View = Reg.view<cmp::Health>();
    for (auto&& [Ent, CHealth] : View) {
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);

        if (!CActive->is_active || CHealth.curr_health > 0)
            continue;
        CActive->is_active = false;
        auto CEnemy = Reg.findComponentOrNull<cmp::IsEnemy>(Ent);
        if (CEnemy) {
            killEnemy(Ent); // enemy health <= 0
        }
        auto CPlayer = Reg.findComponentOrNull<cmp::IsPlayer>(Ent);
        if (CPlayer)
            schedule_quit_game(); // here should be die screen
    }
}
void sys::Regeneration(float dt) {
    auto& View = Reg.view<cmp::Health>();
    for (auto&& [Ent, CHealth] : View) {
        if (!(CHealth.can_regenerate) || (CHealth.curr_health >= CHealth.max_health))
            continue;
        if (CHealth.curr_regeneration_time < CHealth.regeneration_time) {
            CHealth.curr_regeneration_time += dt;
            continue;
        }
        CHealth.curr_health++;
        CHealth.curr_regeneration_time = 0.f;
    }
}
void sys::diaplayPlayerHealth() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerHealth = Reg.findComponentOrNull<cmp::Health>(PlayerEnt);
    auto CPlayerColor = Reg.findComponentOrNull<cmp::Color>(PlayerEnt);
    if ((CPlayerHealth->curr_health >= CPlayerHealth->max_health) || !CPlayerColor)
        return;
    unsigned char red = static_cast<unsigned char>(255 - 255 * CPlayerHealth->curr_health / CPlayerHealth->max_health);
    CPlayerColor->r = red;
    CPlayerColor->g = 255 - red;
}


// Scenario
void sys::updateScenario(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto& View = Reg.view<cmp::Progress>();
    for (auto&& [ScenarioEnt, Progress] : View) {
        auto CEnemies = Reg.findComponentOrNull<cmp::Enemies>(ScenarioEnt);
        auto CPlayerScore = Reg.findComponentOrNull<cmp::IsPlayer>(PlayerEnt);
        if (!CEnemies || !CPlayerScore)
            continue;
        Progress.curr_time += dt;
        Progress.score = CPlayerScore->curr_score;
        if (CPlayerScore->curr_score <= ENEMY_AMOUNT / 2)
            continue;
        CPlayerScore->curr_score = 0;
        for (int i = 0; i < CEnemies->enemy_store.size() - ENEMY_AMOUNT; ++i) {
            EntityId Enemy = CEnemies->enemy_store[i];
            ActivatelEnemy(Enemy);
        }
    }
}
void sys::ActivatelEnemy(EntityId enemy_id) {
    auto CEnemyActive = Reg.findComponentOrNull<cmp::IsActive>(enemy_id);
    auto CEnemyHealth = Reg.findComponentOrNull<cmp::Health>(enemy_id);
    auto CEnemyPos = Reg.findComponentOrNull<cmp::Position>(enemy_id);

    if (CEnemyActive->is_active || !CEnemyHealth || !CEnemyPos)
        return;

    CEnemyActive->is_active = true;
    CEnemyHealth->curr_health = CEnemyHealth->max_health;
    float x_pos = random(BOUND_WIDTH, SCREEN_WIDTH - BOUND_WIDTH);
    float y_pos = random(BOUND_WIDTH, SCREEN_HEIGHT - BOUND_WIDTH);
    CEnemyPos->position = vec2<float>(x_pos, y_pos);

}

//draw(buffer)
void sys::draw(BuffTy Buffer) {

    auto& View = Reg.view<cmp::TriangleSprite>();

    for (auto&& [Ent, Tr] : View) {
        auto CColor = Reg.findComponentOrNull<cmp::Color>(Ent);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);

        if (!CPos || !CColor || !CActive->is_active)
            continue;

        vec2<int> p(static_cast<int>(CPos->position.x), static_cast<int>(CPos->position.y));
        vec2<int> x = get_min_max(Tr.sprite.v1.x, Tr.sprite.v2.x, Tr.sprite.v3.x) + vec2<int>(p.x, p.x); // x = (x_min, x_max)

        vec2<int> y = get_min_max(Tr.sprite.v1.y, Tr.sprite.v2.y, Tr.sprite.v3.y) + vec2<int>(p.y, p.y); // y = (y_min, y_max)

        for (int j = y.x; j <= y.y; ++j) {
            for (int i = x.x; i <= x.y; ++i) {
                if (point_in_triangle(vec2<int>(i, j) - p, Tr.sprite.v1 , Tr.sprite.v2 , Tr.sprite.v3 )) {
                    if (i<0 || i > SCREEN_WIDTH || j < 0 || j > SCREEN_HEIGHT)
                        continue;//out of bounds
                    buffer[j][i] = CColor->data;
                }
            }
        }
    }
}

